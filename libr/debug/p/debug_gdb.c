/* radare - LGPL - Copyright 2009-2023 - pancake, defragger */

#include <r_core.h>
#include <libgdbr.h>
#include <gdbclient/commands.h>

typedef struct {
	libgdbr_t desc;
} RIOGdb;

#define UNKNOWN (-1)
#define UNSUPPORTED 0
#define SUPPORTED 1

static R_TH_LOCAL RIOGdb ** origriogdb = NULL;
static R_TH_LOCAL libgdbr_t *desc = NULL;
static R_TH_LOCAL ut8* reg_buf = NULL;
static R_TH_LOCAL int buf_size = 0;
static R_TH_LOCAL int support_sw_bp = UNKNOWN;
static R_TH_LOCAL int support_hw_bp = UNKNOWN;

static bool r_debug_gdb_attach(RDebug *dbg, int pid);

static void check_connection(RDebug *dbg) {
	if (!desc) {
		r_debug_gdb_attach (dbg, -1);
	}
}

static bool r_debug_gdb_step(RDebug *dbg) {
	check_connection (dbg);
	if (!desc) {
		return false;
	}
	gdbr_step (desc, dbg->tid);
	return true;
}

static RList* r_debug_gdb_threads(RDebug *dbg, int pid) {
	RList *list;
	if ((list = gdbr_threads_list (desc, pid))) {
		list->free = (RListFree) &r_debug_pid_free;
	}
	return list;
}

static RList* r_debug_gdb_pids(RDebug *dbg, int pid) {
	RList *list;
	if ((list = gdbr_pids_list (desc, pid))) {
		list->free = (RListFree) &r_debug_pid_free;
	}
	return list;
}

static bool gdb_reg_read(RDebug *dbg, int type, ut8 *buf, int size) {
	int copy_size;
	int buflen = 0;
	check_connection (dbg);
	if (!desc) {
		return false;
	}
	gdbr_read_registers (desc);
	if (!desc || !desc->data) {
		return false;
	}
	// read the len of the current area
	free (r_reg_get_bytes (dbg->reg, type, &buflen));
	if (size < desc->data_len) {
		R_LOG_WARN ("gdb_reg_read got a small buffer %d vs %d",
			(int)size, (int)desc->data_len);
	}
	copy_size = R_MIN (desc->data_len, size);
	buflen = R_MAX (desc->data_len, buflen);
	if (reg_buf) {
		// if (buf_size < copy_size) { //desc->data_len) {
		if (buflen > buf_size) { //copy_size) {
			ut8* new_buf = realloc (reg_buf, buflen);
			if (!new_buf) {
				return false;
			}
			reg_buf = new_buf;
			buf_size = buflen;
		}
	} else {
		reg_buf = calloc (buflen, 1);
		if (!reg_buf) {
			return false;
		}
		buf_size = buflen;
	}
	memset ((void*)(volatile void*)buf, 0, size);
	memcpy ((void*)(volatile void*)buf, desc->data, R_MIN (copy_size, size));
	memset ((void*)(volatile void*)reg_buf, 0, buflen);
	memcpy ((void*)(volatile void*)reg_buf, desc->data, copy_size);
	// return desc->data_len;
	return true;
}

#ifdef _MSC_VER
#define GDB_FILE_OPEN_MODE (_S_IREAD | _S_IWRITE)
#else
#define GDB_FILE_OPEN_MODE (S_IRUSR | S_IWUSR | S_IXUSR)
#endif

static char *read_remote_maps(ut64 *buflen) {
	*buflen = 0;
	char *fn = r_str_newf ("/proc/%d/maps", desc->pid);
	if (gdbr_open_file (desc, fn, O_RDONLY, GDB_FILE_OPEN_MODE) < 0) {
		free (fn);
		return NULL;
	}
	free (fn);
	int blen = 1024 * 512;
	char *buf = malloc (blen);
	if (buf) {
		int ret = gdbr_read_file (desc, (ut8*)buf, blen);
		if (ret < 1) {
			free (buf);
			return NULL;
		}
		*buflen = ret;
		buf = realloc (buf, ret + 1);
		buf[ret] = 0;
	}
	gdbr_close_file (desc);
	return buf;
}

static RList *r_debug_gdb_map_get(RDebug* dbg) { // TODO
	check_connection (dbg);
	if (!desc || desc->pid <= 0) {
		return NULL;
	}
	RList *retlist = NULL;
	if (desc->get_baddr) {
		desc->get_baddr = false;
		ut64 baddr;
		if ((baddr = gdbr_get_baddr (desc)) != UT64_MAX) {
			if (!(retlist = r_list_new ())) {
				return NULL;
			}
			RDebugMap *map;
			if (!(map = r_debug_map_new ("", baddr, baddr, R_PERM_RX, 0))) {
				r_list_free (retlist);
				return NULL;
			}
			r_list_append (retlist, map);
			return retlist;
		}
	}

	ut64 buflen = 0;
	ut8 *buf = (ut8*) read_remote_maps (&buflen);
	if (!buf) {
		R_LOG_ERROR ("Cannot read /proc/pid/maps");
		return NULL;
	}

	// Get map list
	int unk = 0, perm, i;
	char *ptr, *pos_1;
	size_t line_len;
	char name[1024], region1[256], region2[100], perms[5];
	region1[0] = region2[0] = '0';
	region1[1] = region2[1] = 'x';
	char *save_ptr = NULL;
	if (!(ptr = r_str_tok_r ((char*) buf, "\n", &save_ptr))) {
		free (buf);
		return NULL;
	}
	if (!(retlist = r_list_new ())) {
		free (buf);
		return NULL;
	}
	while (ptr) {
		ut64 map_start, map_end, offset;
		bool map_is_shared = false;
		line_len = strlen (ptr);
		// maps files should not have empty lines
		if (line_len == 0) {
			break;
		}
#if 1
		char *wordstr = strdup (ptr);
		r_str_replace_char (wordstr, '\t', ' ');
		RList *words = r_str_split_list (wordstr, " ", 6);
		int ret = r_list_length (words);
		perms[0] = 0;
		region1[2] = 0;
		region2[2] = 0;
		if (ret > 2) {
			offset = r_num_get (NULL, r_list_get_n (words, 2));
			r_str_ncpy (perms, r_list_get_n (words, 1), sizeof (perms));
			r_str_ncpy (region1 + 2, r_list_get_n (words, 0), sizeof (region1) - 2);
		}
		if (ret > 6) {
			const char *s = r_str_trim_head_ro (r_list_get_n (words, 6));
			r_str_ncpy (name, s, sizeof (name));
		} else {
			*name = 0;
		}
		r_list_free (words);
		free (wordstr);
#else
		// We assume Linux target, for now, so -
		// 7ffff7dda000-7ffff7dfd000 r-xp 00000000 08:05 265428 /usr/lib/ld-2.25.so
		// Android
		// "12c00000-12c40000 rw-p 00000000 00:00 0                                  [anon:dalvik-main space (region space)]";
		ret = sscanf (ptr, "%s %s %"PFMT64x" %*s %*s %[^\n]", &region1[2],
			      perms, &offset, name);
#endif
		// eprintf ("RET = %d (%s)\n", ret, ptr);
		if (ret == 3) {
			name[0] = '\0';
		} else if (ret < 3) {
			R_LOG_WARN ("Cannot parse proc/pid/maps line: %s", buf);
			free (buf);
			r_list_free (retlist);
			return NULL;
		}
		if (!(pos_1 = strchr (&region1[2], '-'))) {
			ptr = r_str_tok_r (NULL, "\n", &save_ptr);
			continue;
		}
		strncpy (&region2[2], pos_1 + 1, sizeof (region2) - 2 - 1);
		if (!*name) {
			snprintf (name, sizeof (name), "unk%d", unk++);
		}
		perm = 0;
		for (i = 0; perms[i] && i < 5; i++) {
			switch (perms[i]) {
			case 'r': perm |= R_PERM_R; break;
			case 'w': perm |= R_PERM_W; break;
			case 'x': perm |= R_PERM_X; break;
			case 'p': map_is_shared = false; break;
			case 's': map_is_shared = true; break;
			}
		}
		map_start = r_num_get (NULL, region1);
		map_end = r_num_get (NULL, region2);
		if (map_start == map_end || map_end == 0) {
			R_LOG_WARN ("%s: ignoring invalid map size: %s - %s", region1, region2);
			ptr = r_str_tok_r (NULL, "\n", &save_ptr);
			continue;
		}
		RDebugMap *map = r_debug_map_new (name, map_start, map_end, perm, 0);
		if (map) {
			map->offset = offset;
			map->shared = map_is_shared;
			map->file = strdup (name);
			r_list_append (retlist, map);
		} else {
			R_LOG_WARN ("Cannot create map 0x%08"PFMT64x" - 0x%08"PFMT64x" (%s)", map_start, map_end, perms);
			break;
		}
		ptr = r_str_tok_r (NULL, "\n", &save_ptr);
	}
	free (buf);
	return retlist;
}

static RList* r_debug_gdb_modules_get(RDebug *dbg) {
	char *lastname = NULL;
	RDebugMap *map;
	RListIter *iter, *iter2;
	RList *list, *last;
	bool must_delete;
	if (!(list = r_debug_gdb_map_get (dbg))) {
		return NULL;
	}
	if (!(last = r_list_newf ((RListFree)r_debug_map_free))) {
		r_list_free (list);
		return NULL;
	}
	r_list_foreach_safe (list, iter, iter2, map) {
		const char *file = map->file;
		if (!map->file) {
			file = map->file = strdup (map->name);
		}
		must_delete = true;
		if (file && *file == '/') {
			if (!lastname || strcmp (lastname, file)) {
				must_delete = false;
			}
		}
		if (must_delete) {
			r_list_delete (list, iter);
		} else {
			r_list_append (last, map);
			free (lastname);
			lastname = strdup (file);
		}
	}
	list->free = NULL;
	free (lastname);
	r_list_free (list);
	return last;
}

static bool gdb_reg_write(RDebug *dbg, int type, const ut8 *buf, int size) {
	check_connection (dbg);
	if (!desc) {
		return false;
	}
	if (!reg_buf) {
		// we cannot write registers before we once read them
		return false;
	}
	int buflen = 0;
	int bits = dbg->anal->config->bits;
	const char *pcname = r_reg_get_name (dbg->anal->reg, R_REG_NAME_PC);
	RRegItem *reg = r_reg_get (dbg->anal->reg, pcname, 0);
	if (reg) {
		if (bits != reg->size) {
			bits = reg->size;
		}
	}
	free (r_reg_get_bytes (dbg->reg, type, &buflen));
	// some implementations of the gdb protocol are acting weird.
	// so winedbg is not able to write registers through the <G> packet
	// and also it does not return the whole gdb register profile after
	// calling <g>
	// so this workaround resizes the small register profile buffer
	// to the whole set and fills the rest with 0
	if (buf_size < buflen) {
		ut8* new_buf = realloc (reg_buf, buflen * sizeof (ut8));
		if (!new_buf) {
			return false;
		}
		reg_buf = new_buf;
		memset (new_buf + buf_size, 0, buflen - buf_size);
	}

	RRegItem* current = NULL;
	// We default to little endian if there's no way to get the configuration,
	// since this was the behaviour prior to the change.
	RRegArena *arena = dbg->reg->regset[type].arena;
	for (;;) {
		current = r_reg_next_diff (dbg->reg, type, reg_buf, buflen, current, bits);
		if (!current) {
			break;
		}
		gdbr_write_reg (desc, current->name, (char*)arena->bytes + (current->offset / 8), current->size / 8);
	}
	return true;
}

static bool r_debug_gdb_continue(RDebug *dbg, int pid, int tid, int sig) {
	check_connection (dbg);
	if (!desc) {
		return false;
	}
	gdbr_continue (desc, pid, -1, sig); // Continue all threads
	if (desc->stop_reason.is_valid && desc->stop_reason.thread.present) {
		//if (desc->tid != desc->stop_reason.thread.tid) {
		//	eprintf ("thread id (%d) in reason differs from current thread id (%d)\n", dbg->pid, dbg->tid);
		//}
		desc->tid = desc->stop_reason.thread.tid;
	}
	dbg->tid = desc->tid;
	return true;
}

static RDebugReasonType r_debug_gdb_wait(RDebug *dbg, int pid) {
	check_connection (dbg);
	if (!desc) {
		return R_DEBUG_REASON_UNKNOWN;
	}
	if (!desc->stop_reason.is_valid) {
		if (gdbr_stop_reason (desc) < 0) {
			dbg->reason.type = R_DEBUG_REASON_UNKNOWN;
			return R_DEBUG_REASON_UNKNOWN;
		}
	}
	if (desc->stop_reason.thread.present) {
		dbg->reason.tid = desc->stop_reason.thread.tid;
		dbg->pid = desc->stop_reason.thread.pid;
		dbg->tid = desc->stop_reason.thread.tid;
		if (dbg->pid != desc->pid || dbg->tid != desc->tid) {
			//eprintf ("= attach %d %d\n", dbg->pid, dbg->tid);
			gdbr_select (desc, dbg->pid, dbg->tid);
		}
	}
	dbg->reason.signum = desc->stop_reason.signum;
	dbg->reason.type = desc->stop_reason.reason;
	return desc->stop_reason.reason;
}

static bool r_debug_gdb_attach(RDebug *dbg, int pid) {
	RIODesc *d = dbg->iob.io->desc;
	// TODO: the core must update the dbg.swstep config var when this var is changed
	dbg->swstep = false;
	if (d && d->plugin && d->plugin->name && d->data) {
		if (!strcmp ("gdb", d->plugin->name)) {
			RIOGdb *g = d->data;
			origriogdb = (RIOGdb **)&d->data;	//TODO bit of a hack, please improve
			support_sw_bp = UNKNOWN;
			support_hw_bp = UNKNOWN;
			desc = &g->desc;
			int arch = r_sys_arch_id (dbg->arch);
			int bits = dbg->anal->config->bits;
			gdbr_set_architecture (desc, arch, bits);
		} else {
			R_LOG_ERROR ("Underlying IO descriptor is not a GDB one");
		}
	}
	return true;
}

static bool r_debug_gdb_detach(RDebug *dbg, int pid) {
	bool ret = false;

	if (pid <= 0 || !desc->stub_features.multiprocess) {
		ret = gdbr_detach (desc);
	}
	ret = gdbr_detach_pid (desc, pid);

	if (dbg->pid == pid) {
		desc = NULL;
	}
	return ret;
}

static const char *r_debug_gdb_reg_profile(RDebug *dbg) {
	check_connection (dbg);
	int arch = r_sys_arch_id (dbg->arch);
	int bits = dbg->anal->config->bits;
	// XXX This happens when radare2 set dbg.backend before opening io_gdb
	if (!desc) {
		return gdbr_get_reg_profile (arch, bits);
	}
	if (!desc->target.valid) {
		gdbr_set_architecture (desc, arch, bits);
	}
	if (desc->target.regprofile) {
		return strdup (desc->target.regprofile);
	}
	return NULL;
}

static int r_debug_gdb_set_reg_profile(const char *str) {
	if (desc && str) {
		return gdbr_set_reg_profile (desc, str);
	}
	return false;
}

static int r_debug_gdb_breakpoint(RBreakpoint *bp, RBreakpointItem *b, bool set) {
	int ret = 0, bpsize;
	if (!b) {
		return false;
	}
	bpsize = b->size;
	// TODO handle conditions
	switch (b->perm) {
	case R_BP_PROT_EXEC : {
		if (set) {
			ret = b->hw?
					gdbr_set_hwbp (desc, b->addr, "", bpsize):
					gdbr_set_bp (desc, b->addr, "", bpsize);
		} else {
			ret = b->hw ? gdbr_remove_hwbp (desc, b->addr, bpsize) : gdbr_remove_bp (desc, b->addr, bpsize);
		}
		break;
	}
	// TODO handle size (area of watch in upper layer and then bpsize. For the moment watches are set on exact on byte
	case R_PERM_W: {
		if (set) {
			gdbr_set_hww (desc, b->addr, "", 1);
		} else {
			gdbr_remove_hww (desc, b->addr, 1);
		}
		break;
	}
	case R_PERM_R: {
		if (set) {
			gdbr_set_hwr (desc, b->addr, "", 1);
		} else {
			gdbr_remove_hwr (desc, b->addr, 1);
		}
		break;
	}
	case R_PERM_ACCESS: {
		if (set) {
			gdbr_set_hwa (desc, b->addr, "", 1);
		} else {
			gdbr_remove_hwa (desc, b->addr, 1);
		}
		break;
	}
	}
	return !ret;
}

static bool r_debug_gdb_kill(RDebug *dbg, int pid, int tid, int sig) {
	// TODO kill based on pid and signal
	if (sig != 0) {
		if (gdbr_kill (desc) < 0) {
			return false;
		}
	}
	return true;
}

static bool r_debug_gdb_select(RDebug *dbg, int pid, int tid) {
	if (!desc || !*origriogdb) {
		desc = NULL;	//TODO hacky fix, please improve. I would suggest using a **desc instead of a *desc, so it is automatically updated
		return false;
	}

	int child = gdbr_select (desc, pid, tid);
	if (child != -1) {
		dbg->tid = child;
		return true;
	}
	return false;
}

static RDebugInfo* r_debug_gdb_info(RDebug *dbg, const char *arg) {
	RDebugInfo *rdi;
	if (!(rdi = R_NEW0 (RDebugInfo))) {
		return NULL;
	}
	RList *th_list;
	bool list_alloc = false;
	if (dbg->threads) {
		th_list = dbg->threads;
	} else {
		th_list = r_debug_gdb_threads (dbg, dbg->pid);
		list_alloc = true;
	}
	RDebugPid *th;
	RListIter *it;
	bool found = false;
	r_list_foreach (th_list, it, th) {
		if (th->pid == dbg->pid) {
			found = true;
			break;
		}
	}
	rdi->pid = dbg->pid;
	rdi->tid = dbg->tid;
	rdi->exe = gdbr_exec_file_read (desc, dbg->pid);
	rdi->status = found ? th->status : R_DBG_PROC_STOP;
	rdi->uid = found ? th->uid : -1;
	rdi->gid = found ? th->gid : -1;
	if (gdbr_stop_reason (desc) >= 0) {
		R_LOG_INFO ("signal: %d", desc->stop_reason.signum);
		rdi->signum = desc->stop_reason.signum;
	}
	if (list_alloc) {
		r_list_free (th_list);
	}
	return rdi;
}

#include "native/bt.c"

static RList* r_debug_gdb_frames(RDebug *dbg, ut64 at) {
	return r_debug_native_frames (dbg, at);
}

RDebugPlugin r_debug_plugin_gdb = {
	.name = "gdb",
	/* TODO: Add support for more architectures here */
	.license = "LGPL3",
	.arch = "x86,arm,sh,mips,avr,lm32,v850,ba2",
	.bits = R_SYS_BITS_16 | R_SYS_BITS_32 | R_SYS_BITS_64,
	.step = r_debug_gdb_step,
	.cont = r_debug_gdb_continue,
	.attach = &r_debug_gdb_attach,
	.detach = r_debug_gdb_detach,
	.threads = r_debug_gdb_threads,
	.pids = r_debug_gdb_pids,
	.canstep = 1,
	.wait = r_debug_gdb_wait,
	.map_get = r_debug_gdb_map_get,
	.modules_get = r_debug_gdb_modules_get,
	.breakpoint = &r_debug_gdb_breakpoint,
	.reg_read = &gdb_reg_read,
	.reg_write = &gdb_reg_write,
	.reg_profile = (void *)r_debug_gdb_reg_profile,
	.set_reg_profile = &r_debug_gdb_set_reg_profile,
	.kill = &r_debug_gdb_kill,
	.info = &r_debug_gdb_info,
	.select = &r_debug_gdb_select,
	.frames = &r_debug_gdb_frames,
	//.bp_write = &r_debug_gdb_bp_write,
	//.bp_read = &r_debug_gdb_bp_read,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_DBG,
	.data = &r_debug_plugin_gdb,
	.version = R2_VERSION
};
#endif

