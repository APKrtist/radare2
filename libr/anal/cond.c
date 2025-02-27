/* radare - LGPL - Copyright 2010-2023 - pancake */

#include <r_anal.h>

R_API const char *r_anal_cond_type_tostring(int cc) {
	switch (cc) {
	case R_ANAL_COND_EQ: return "eq";
	case R_ANAL_COND_NV: return "nv";
	case R_ANAL_COND_NE: return "ne";
	case R_ANAL_COND_HS: return "hs";
	case R_ANAL_COND_LO: return "lo";
	case R_ANAL_COND_MI: return "mi";
	case R_ANAL_COND_PL: return "pl";
	case R_ANAL_COND_VS: return "vs";
	case R_ANAL_COND_VC: return "vc";
	case R_ANAL_COND_HI: return "hi";
	case R_ANAL_COND_LS: return "ls";
	case R_ANAL_COND_GE: return "ge";
	case R_ANAL_COND_LT: return "lt";
	case R_ANAL_COND_GT: return "gt";
	case R_ANAL_COND_LE: return "le";
	case R_ANAL_COND_AL: return "al";
	}
	return "??";
}

R_API RAnalCond *r_anal_cond_new(void) {
	return R_NEW0 (RAnalCond);
}

R_API void r_anal_cond_fini(RAnalCond *c) {
	if (!c) {
		return;
	}
	r_anal_value_free (c->arg[0]);
	r_anal_value_free (c->arg[1]);
	c->arg[0] = c->arg[1] = NULL;
}

R_API void r_anal_cond_free(RAnalCond *c) {
	if (!c) {
		return;
	}
	r_anal_cond_fini (c);
	free (c);
}

// XXX?
R_API RAnalCond *r_anal_cond_clone(RAnalCond *cond) {
	RAnalCond *c = R_NEW (RAnalCond);
	if (!c) {
		return NULL;
	}
	memcpy (c, cond, sizeof (RAnalCond));
	return c;
}

static inline const char *condstring(RAnalCond *cond) {
	const char *condstr_single[] = { "!", "", "0<", "0<=", "0>", "0>=" };
	const char *condstr[] = { "==", "!=", ">=", ">", "<=", "<" };
	if (cond) {
		if (cond->arg[1]) {
			return condstr[cond->type % 6];
		} else {
			return condstr_single[cond->type % 6];
		}
	}
	return "";
}

R_API int r_anal_cond_eval(RAnal *anal, RAnalCond *cond) {
	// XXX: sign issue here?
	st64 arg0 = (st64) r_anal_value_to_ut64 (anal, cond->arg[0]);
	if (cond->arg[1]) {
		st64 arg1 = (st64) r_anal_value_to_ut64 (anal, cond->arg[1]);
		switch (cond->type) {
		case R_ANAL_COND_EQ: return arg0 == arg1;
		case R_ANAL_COND_NE: return arg0 != arg1;
		case R_ANAL_COND_GE: return arg0 >= arg1;
		case R_ANAL_COND_GT: return arg0 > arg1;
		case R_ANAL_COND_LE: return arg0 <= arg1;
		case R_ANAL_COND_LT: return arg0 < arg1;
		}
	} else {
		switch (cond->type) {
		case R_ANAL_COND_EQ: return !arg0;
		case R_ANAL_COND_NE: return arg0;
		case R_ANAL_COND_GT: return arg0 > 0;
		case R_ANAL_COND_GE: return arg0 >= 0;
		case R_ANAL_COND_LT: return arg0 < 0;
		case R_ANAL_COND_LE: return arg0 <= 0;
		}
	}
	return false;
}

R_API char *r_anal_cond_tostring(RAnalCond *cond) {
	r_return_val_if_fail (cond, NULL);
	const char *cnd = condstring (cond);
	char *val0 = r_anal_value_tostring (cond->arg[0]);
	char *out = NULL;
	if (val0) {
		if (R_ANAL_COND_SINGLE (cond)) {
			out = r_str_newf ("%s%s", cnd, val0);
		} else {
			char *val1 = r_anal_value_tostring (cond->arg[1]);
			if (val1) {
				out = r_str_newf ("%s %s %s", val0, cnd, val1);
				free (val1);
			}
		}
		free (val0);
	}
	return out? out: strdup ("?");
}

R_API RAnalCond *r_anal_cond_new_from_op(RAnalOp *op) {
	RAnalCond *cond;
	if (!(cond = r_anal_cond_new ())) {
		return NULL;
	}
	RAnalValue *src0 = r_vector_at (&op->srcs, 0);
	RAnalValue *src1 = r_vector_at (&op->srcs, 1);
	if (!src0 || !src1) {
		r_anal_cond_free (cond);
		return NULL;
	}
	// TODO: use r_ref
	cond->arg[0] = r_anal_value_clone (src0);
	cond->arg[1] = r_anal_value_clone (src1);
	return cond;
}

R_API RAnalCond *r_anal_cond_new_from_string(const char *str) {
	RAnalCond *cond = R_NEW0 (RAnalCond);
	// TODO: find '<','=','>','!'...
	return cond;
}
