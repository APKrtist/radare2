static const char *const js_repl_qjs = "" \
  "import(\"os\").catch(console.error).then((os=>{!function(g){g.o"\
  "s=os;let running=!0;const Object=g.Object,String=g.String,Arr"\
  "ay=g.Array,Date=g.Date,Math=g.Math,isFinite=g.isFinite,parseF"\
  "loat=g.parseFloat,config_numcalc=!1,has_jscalc=\"function\"==ty"\
  "peof Fraction,has_bignum=\"function\"==typeof BigFloat,colors={"\
  "none:\"""\x1b""[0m\",black:\"""\x1b""[30m\",red:\"""\x1b""[31m\",green:\"""\x1b""[32"\
  "m\",yellow:\"""\x1b""[33m\",blue:\"""\x1b""[34m\",magenta:\"""\x1b""[35m\",cyan:"\
  "\"""\x1b""[36m\",white:\"""\x1b""[37m\",gray:\"""\x1b""[30;1m\",grey:\"""\x1b""[30;1"\
  "m\",bright_red:\"""\x1b""[31;1m\",bright_green:\"""\x1b""[32;1m\",bright_y"\
  "ellow:\"""\x1b""[33;1m\",bright_blue:\"""\x1b""[34;1m\",bright_magenta:\"""\x1b"""\
  "[35;1m\",bright_cyan:\"""\x1b""[36;1m\",bright_white:\"""\x1b""[37;1m\"};l"\
  "et styles;styles=config_numcalc?{default:\"black\",comment:\"whi"\
  "te\",string:\"green\",regex:\"cyan\",number:\"green\",keyword:\"blue\""\
  ",function:\"gray\",type:\"magenta\",identifier:\"yellow\",error:\"br"\
  "ight_red\",result:\"black\",error_msg:\"bright_red\"}:{default:\"wh"\
  "ite\",comment:\"white\",string:\"cyan\",regex:\"cyan\",number:\"green"\
  "\",keyword:\"magenta\",function:\"yellow\",type:\"magenta\",identifi"\
  "er:\"green\",error:\"red\",result:\"bright_white\",error_msg:\"brigh"\
  "t_red\"};const history=[];let clip_board=\"\",prec,expBits,log2_"\
  "10,pstate=\"\",prompt=\"\",plen=0,ps1;ps1=config_numcalc?\"> \":\"[r"\
  "2js]> \";const ps2=\" ... \",utf8=!0;let show_time=!1,show_color"\
  "s=!0,eval_time=0,mexpr=\"\",level=0,cmd=\"\",cursor_pos=0,last_cm"\
  "d=\"\",last_cursor_pos=0,history_index,this_fun,last_fun,quote_"\
  "flag=!1,utf8_state=0,utf8_val=0,term_fd,term_read_buf,term_wi"\
  "dth,term_cursor_x=0;function termInit(){for(term_fd=0,(term_w"\
  "idth=+r2cmd(\"?vi $c\"))<1&&(term_width=80),term_read_buf=new U"\
  "int8Array(64),cmd_start(),running=!0;running;)try{term_read_h"\
  "andler(),flush()}catch(e){console.error(e)}}function term_rea"\
  "d_handler(){var e=os.read(term_fd,term_read_buf.buffer,0,term"\
  "_read_buf.length);for(let r=0;r<e;r++)handle_byte(term_read_b"\
  "uf[r])}function handle_byte(e){utf8?0!==utf8_state&&128<=e&&e"\
  "<192?(utf8_val=utf8_val<<6|63&e,0==--utf8_state&&handle_char("\
  "utf8_val)):192<=e&&e<248?(utf8_state=1+(224<=e)+(240<=e),utf8"\
  "_val=e&(1<<6-utf8_state)-1):(utf8_state=0,handle_char(e)):han"\
  "dle_char(e)}function is_alpha(e){return\"string\"==typeof e&&(\""\
  "A\"<=e&&e<=\"Z\"||\"a\"<=e&&e<=\"z\")}function is_digit(e){return\"st"\
  "ring\"==typeof e&&\"0\"<=e&&e<=\"9\"}function is_word(e){return\"st"\
  "ring\"==typeof e&&(is_alpha(e)||is_digit(e)||\"_\"===e||\"$\"===e)"\
  "}function ucs_length(e){let r,t,n;var o=e.length;for(r=0,n=0;"\
  "n<o;n++)((t=e.charCodeAt(n))<56320||57344<=t)&&r++;return r}f"\
  "unction is_trailing_surrogate(e){if(\"string\"==typeof e)return"\
  " 56320<=(e=e.codePointAt(0))&&e<57344}function is_balanced(e,"\
  "r){switch(e+r){case\"()\":case\"[]\":case\"{}\":return 1}}function "\
  "print_color_text(e,r,t){let n,o;for(o=r;o<e.length;){const r="\
  "t[n=o];for(;++o<e.length&&t[o]===r;);write(colors[styles[r]||"\
  "\"default\"]),write(e.substring(n,o)),write(colors.none)}}funct"\
  "ion print_csi(e,r){write(\"""\x1b""[\"+(1!==e?e:\"\")+r)}function mov"\
  "e_cursor(e){let r;if(0<e)for(;0<e;)term_cursor_x===term_width"\
  "-1?(write(\"\\n\"),term_cursor_x=0,e--):(print_csi(r=Math.min(te"\
  "rm_width-1-term_cursor_x,e),\"C\"),e-=r,term_cursor_x+=r);else "\
  "for(e=-e;0<e;)0===term_cursor_x?(print_csi(1,\"A\"),print_csi(t"\
  "erm_width-1,\"C\"),e--,term_cursor_x=term_width-1):(print_csi(r"\
  "=Math.min(e,term_cursor_x),\"D\"),e-=r,term_cursor_x-=r)}functi"\
  "on update(){var e;cmd!==last_cmd&&(show_colors||last_cmd.subs"\
  "tring(0,last_cursor_pos)!==cmd.substring(0,last_cursor_pos)?("\
  "move_cursor(-ucs_length(last_cmd.substring(0,last_cursor_pos)"\
  ")),show_colors?print_color_text(e=mexpr?mexpr+\"\\n\"+cmd:cmd,e."\
  "length-cmd.length,colorize_js(e)[2]):write(cmd)):write(cmd.su"\
  "bstring(last_cursor_pos)),0==(term_cursor_x=(term_cursor_x+uc"\
  "s_length(cmd))%term_width)&&write(\" \\b\"),write(\"""\x1b""[J\"),last"\
  "_cmd=cmd,last_cursor_pos=cmd.length),cursor_pos>last_cursor_p"\
  "os?move_cursor(ucs_length(cmd.substring(last_cursor_pos,curso"\
  "r_pos))):cursor_pos<last_cursor_pos&&move_cursor(-ucs_length("\
  "cmd.substring(cursor_pos,last_cursor_pos))),last_cursor_pos=c"\
  "ursor_pos,flush()}function insert(e){e&&(cmd=cmd.substring(0,"\
  "cursor_pos)+e+cmd.substring(cursor_pos),cursor_pos+=e.length)"\
  "}function quoted_insert(){quote_flag=!0}function abort(){retu"\
  "rn cmd=\"\",cursor_pos=0,-2}function alert(){}function beginnin"\
  "g_of_line(){cursor_pos=0}function end_of_line(){cursor_pos=cm"\
  "d.length}function forward_char(){if(cursor_pos<cmd.length)for"\
  "(cursor_pos++;is_trailing_surrogate(cmd.charAt(cursor_pos));)"\
  "cursor_pos++}function backward_char(){if(0<cursor_pos)for(cur"\
  "sor_pos--;is_trailing_surrogate(cmd.charAt(cursor_pos));)curs"\
  "or_pos--}function skip_word_forward(e){for(;e<cmd.length&&!is"\
  "_word(cmd.charAt(e));)e++;for(;e<cmd.length&&is_word(cmd.char"\
  "At(e));)e++;return e}function skip_word_backward(e){for(;0<e&"\
  "&!is_word(cmd.charAt(e-1));)e--;for(;0<e&&is_word(cmd.charAt("\
  "e-1));)e--;return e}function forward_word(){cursor_pos=skip_w"\
  "ord_forward(cursor_pos)}function backward_word(){cursor_pos=s"\
  "kip_word_backward(cursor_pos)}function accept_line(){return w"\
  "rite(\"\\n\"),history_add(cmd),-1}function history_add(e){e&&his"\
  "tory.push(e),history_index=history.length}function previous_h"\
  "istory(){0<history_index&&(history_index===history.length&&hi"\
  "story.push(cmd),history_index--,cmd=history[history_index],cu"\
  "rsor_pos=cmd.length)}function next_history(){history_index<hi"\
  "story.length-1&&(history_index++,cmd=history[history_index],c"\
  "ursor_pos=cmd.length)}function history_search(e){var r=cursor"\
  "_pos;for(let n=1;n<=history.length;n++){var t=(history.length"\
  "+n*e+history_index)%history.length;if(history[t].substring(0,"\
  "r)===cmd.substring(0,r))return history_index=t,void(cmd=histo"\
  "ry[t])}}function history_search_backward(){return history_sea"\
  "rch(-1)}function history_search_forward(){return history_sear"\
  "ch(1)}function delete_char_dir(e){let r,t;if(r=cursor_pos,e<0"\
  ")for(r--;is_trailing_surrogate(cmd.charAt(r));)r--;for(t=r+1;"\
  "is_trailing_surrogate(cmd.charAt(t));)t++;0<=r&&r<cmd.length&"\
  "&(last_fun===kill_region?kill_region(r,t,e):(cmd=cmd.substrin"\
  "g(0,r)+cmd.substring(t),cursor_pos=r))}function delete_char()"\
  "{delete_char_dir(1)}function control_d(){if(0===cmd.length)re"\
  "turn write(\"\\n\"),running=!1,-3;delete_char_dir(1)}function ba"\
  "ckward_delete_char(){delete_char_dir(-1)}function transpose_c"\
  "hars(){let e=cursor_pos;1<cmd.length&&0<e&&(e===cmd.length&&e"\
  "--,cmd=cmd.substring(0,e-1)+cmd.substring(e,e+1)+cmd.substrin"\
  "g(e-1,e)+cmd.substring(e+1),cursor_pos=e+1)}function transpos"\
  "e_words(){var e=skip_word_backward(cursor_pos),r=skip_word_fo"\
  "rward(e),t=skip_word_forward(cursor_pos),n=skip_word_backward"\
  "(t);e<r&&r<=cursor_pos&&cursor_pos<=n&&n<t&&(cmd=cmd.substrin"\
  "g(0,e)+cmd.substring(n,t)+cmd.substring(r,n)+cmd.substring(e,"\
  "r),cursor_pos=t)}function upcase_word(){var e=skip_word_forwa"\
  "rd(cursor_pos);cmd=cmd.substring(0,cursor_pos)+cmd.substring("\
  "cursor_pos,e).toUpperCase()+cmd.substring(e)}function downcas"\
  "e_word(){var e=skip_word_forward(cursor_pos);cmd=cmd.substrin"\
  "g(0,cursor_pos)+cmd.substring(cursor_pos,e).toLowerCase()+cmd"\
  ".substring(e)}function kill_region(e,r,t){var n=cmd.substring"\
  "(e,r);last_fun!==kill_region?clip_board=n:t<0?clip_board=n+cl"\
  "ip_board:clip_board+=n,cmd=cmd.substring(0,e)+cmd.substring(r"\
  "),cursor_pos>r?cursor_pos-=r-e:cursor_pos>e&&(cursor_pos=e),t"\
  "his_fun=kill_region}function kill_line(){kill_region(cursor_p"\
  "os,cmd.length,1)}function backward_kill_line(){kill_region(0,"\
  "cursor_pos,-1)}function kill_word(){kill_region(cursor_pos,sk"\
  "ip_word_forward(cursor_pos),1)}function backward_kill_word(){"\
  "kill_region(skip_word_backward(cursor_pos),cursor_pos,-1)}fun"\
  "ction yank(){insert(clip_board)}function control_c(){console."\
  "log(\"^C\"),reset(),readline_print_prompt()}function reset(){cm"\
  "d=\"\",cursor_pos=0}function get_context_word(e,r){let t=\"\";for"\
  "(;0<r&&is_word(e[r-1]);)t=e[--r]+t;return t}function get_cont"\
  "ext_object(line,pos){let obj,base,c;if(pos<=0||0<=\" ~!%^&*(-+"\
  "={[|:;,<>?/\".indexOf(line[pos-1]))return g;if(2<=pos&&\".\"===l"\
  "ine[pos-1])switch(pos--,obj={},c=line[pos-1]){case\"'\":case'\"'"\
  ":return\"a\";case\"]\":return[];case\"}\":return{};case\"/\":return/ "\
  "/;default:return is_word(c)?(base=get_context_word(line,pos),"\
  "[\"true\",\"false\",\"null\",\"this\"].includes(base)||!isNaN(+base)?"\
  "eval(base):(obj=get_context_object(line,pos-base.length),null"\
  "==obj?obj:obj===g&&void 0===obj[base]?eval(base):obj[base])):"\
  "{}}}function get_completions(e,r){let t,n,o;var i=get_context"\
  "_word(e,r),s=[];for(n=0,t=e=get_context_object(e,r-i.length);"\
  "n<10&&null!=t;n++){const e=Object.getOwnPropertyNames(t);for("\
  "o=0;o<e.length;o++){const r=e[o];\"string\"==typeof r&&\"\"+ +r!="\
  "=r&&r.startsWith(i)&&s.push(r)}t=Object.getPrototypeOf(t)}if("\
  "1<s.length){for(s.sort((function(e,r){if(e[0]!==r[0]){if(\"_\"="\
  "==e[0])return 1;if(\"_\"===r[0])return-1}return e<r?-1:r<e?1:0}"\
  ")),n=o=1;n<s.length;n++)s[n]!==s[n-1]&&(s[o++]=s[n]);s.length"\
  "=o}return{tab:s,pos:i.length,ctx:e}}function completion(){let"\
  " e,r,t,n,o,i,s,c,a,l;var _=get_completions(cmd,cursor_pos),u="\
  "_.tab;if(0!==u.length){for(e=u[0],n=e.length,r=1;r<u.length;r"\
  "++)for(o=u[r],t=0;t<n;t++)if(o[t]!==e[t]){n=t;break}for(r=_.p"\
  "os;r<n;r++)insert(e[r]);if(last_fun===completion&&1===u.lengt"\
  "h){const e=_.ctx[u[0]];\"function\"==typeof e?(insert(\"(\"),0==="\
  "e.length&&insert(\")\")):\"object\"==typeof e&&insert(\".\")}if(las"\
  "t_fun===completion&&2<=u.length){for(i=0,r=0;r<u.length;r++)i"\
  "=Math.max(i,u[r].length);for(i+=2,c=Math.max(1,Math.floor((te"\
  "rm_width+1)/i)),l=Math.ceil(u.length/c),write(\"\\n\"),a=0;a<l;a"\
  "++){for(s=0;s<c&&!((r=s*l+a)>=u.length);s++)e=u[r],s!==c-1&&("\
  "e=e.padEnd(i)),write(e);write(\"\\n\")}readline_print_prompt()}}"\
  "}const commands={\"""\x01""\":beginning_of_line,\"""\x02""\":backward_cha"\
  "r,\"""\x03""\":control_c,\"""\x04""\":control_d,\"""\x05""\":end_of_line,\"""\x06""\""\
  ":forward_char,\"""\x07""\":abort,\"\\b\":backward_delete_char,\"\\t\":com"\
  "pletion,\"\\n\":accept_line,\"\\v\":kill_line,\"\\r\":accept_line,\"""\x0e"""\
  "\":next_history,\"""\x10""\":previous_history,\"""\x11""\":quoted_insert,\""\
  """\x12""\":alert,\"""\x13""\":alert,\"""\x14""\":transpose_chars,\"""\x17""\":backwa"\
  "rd_kill_word,\"""\x18""\":reset,\"""\x19""\":yank,\"""\x1b""OA\":previous_histo"\
  "ry,\"""\x1b""OB\":next_history,\"""\x1b""OC\":forward_char,\"""\x1b""OD\":backw"\
  "ard_char,\"""\x1b""OF\":forward_word,\"""\x1b""OH\":backward_word,\"""\x1b""[1"\
  ";5C\":forward_word,\"""\x1b""[1;5D\":backward_word,\"""\x1b""[1~\":beginni"\
  "ng_of_line,\"""\x1b""[3~\":delete_char,\"""\x1b""[4~\":end_of_line,\"""\x1b""["\
  "5~\":history_search_backward,\"""\x1b""[6~\":history_search_forward,"\
  "\"""\x1b""[A\":previous_history,\"""\x1b""[B\":next_history,\"""\x1b""[C\":forw"\
  "ard_char,\"""\x1b""[D\":backward_char,\"""\x1b""[F\":end_of_line,\"""\x1b""[H\""\
  ":beginning_of_line,\"""\x1b""""\x7f""\":backward_kill_word,\"""\x1b""b\":back"\
  "ward_word,\"""\x1b""d\":kill_word,\"""\x1b""f\":forward_word,\"""\x1b""k\":back"\
  "ward_kill_line,\"""\x1b""l\":downcase_word,\"""\x1b""t\":transpose_words,"\
  "\"""\x1b""u\":upcase_word,\"""\x7f""\":backward_delete_char};function dup"\
  "str(e,r){let t=\"\";for(;0<r--;)t+=e;return t}let readline_keys"\
  ",readline_state,readline_cb;function readline_print_prompt(){"\
  "write(prompt),term_cursor_x=ucs_length(prompt)%term_width,las"\
  "t_cmd=\"\",last_cursor_pos=0}function readline_start(e,r){if(cm"\
  "d=e||\"\",cursor_pos=cmd.length,history_index=history.length,re"\
  "adline_cb=r,prompt=pstate,mexpr)prompt=(prompt+=dupstr(\" \",pl"\
  "en-prompt.length))+ps2;else{if(show_time){let e=Math.round(ev"\
  "al_time)+\" \";eval_time=0,e=dupstr(\"0\",5-e.length)+e,prompt+=e"\
  ".substring(0,e.length-4)+\".\"+e.substring(e.length-4)}plen=pro"\
  "mpt.length,show_colors&&(prompt+=colors.yellow),prompt+=ps1,s"\
  "how_colors&&(prompt+=colors.none)}readline_print_prompt(),upd"\
  "ate(),readline_state=0}function handle_char(e){var r=String.f"\
  "romCodePoint(e);switch(readline_state){case 0:\"""\x1b""\"===r?(rea"\
  "dline_keys=r,readline_state=1):handle_key(r);break;case 1:rea"\
  "dline_keys+=r,readline_state=\"[\"===r?2:\"O\"===r?3:(handle_key("\
  "readline_keys),0);break;case 2:readline_keys+=r,\";\"===r||\"0\"<"\
  "=r&&r<=\"9\"||(handle_key(readline_keys),readline_state=0);brea"\
  "k;case 3:handle_key(readline_keys+=r),readline_state=0}}funct"\
  "ion handle_key(e){var r;if(quote_flag)1===ucs_length(e)&&inse"\
  "rt(e),quote_flag=!1;else if(r=commands[e]){switch((this_fun=r"\
  ")(e)){case-1:return readline_cb(cmd);case-2:return readline_c"\
  "b(null);case-3:return}last_fun=this_fun}else 1===ucs_length(e"\
  ")&&\" \"<=e?(insert(e),last_fun=insert):alert();cursor_pos=curs"\
  "or_pos<0?0:cursor_pos>cmd.length?cmd.length:cursor_pos,update"\
  "()}let hex_mode=!1,eval_mode=\"std\";function number_to_string("\
  "e,r){if(isFinite(e)){let t;return 0===e?t=1/e<0?\"-0\":\"0\":16=="\
  "=r&&e===Math.floor(e)?(t=e<0?(e=-e,\"-\"):\"\",t+=\"0x\"+e.toString"\
  "(16)):t=e.toString(),t}return e.toString()}function bigfloat_"\
  "to_string(e,r){let t;return BigFloat.isFinite(e)?(0===e?t=1/e"\
  "<0?\"-0\":\"0\":16===r?(t=e<0?(e=-e,\"-\"):\"\",t+=\"0x\"+e.toString(16"\
  ")):t=e.toString(),\"bigfloat\"==typeof e&&\"math\"!==eval_mode?t+"\
  "=\"l\":\"std\"!==eval_mode&&t.indexOf(\".\")<0&&(16===r&&t.indexOf("\
  "\"p\")<0||10===r&&t.indexOf(\"e\")<0)&&(t+=\".0\"),t):\"math\"!==eval"\
  "_mode?\"BigFloat(\"+e.toString()+\")\":e.toString()}function bigi"\
  "nt_to_string(e,r){let t;return 16===r?(t=e<0?(e=-e,\"-\"):\"\",t+"\
  "=\"0x\"+e.toString(16)):t=e.toString(),\"std\"===eval_mode&&(t+=\""\
  "n\"),t}function print(e){const r=[];!function e(t){let n,o,i,s"\
  ",c;var a=typeof t;if(\"object\"==a)if(null===t)write(t);else if"\
  "(0<=r.indexOf(t))write(\"[circular]\");else if(has_jscalc&&(t i"\
  "nstanceof Fraction||t instanceof Complex||t instanceof Mod||t"\
  " instanceof Polynomial||t instanceof PolyMod||t instanceof Ra"\
  "tionalFunction||t instanceof Series))write(t.toString());else"\
  "{if(r.push(t),Array.isArray(t)){for(n=t.length,write(\"[ \"),o="\
  "0;o<n;o++)if(0!==o&&write(\", \"),o in t?e(t[o]):write(\"<empty>"\
  "\"),20<o){write(\"...\");break}write(\" ]\")}else if(\"RegExp\"===Ob"\
  "ject.__getClass(t))write(t.toString());else{for(i=Object.keys"\
  "(t),n=i.length,write(\"{ \"),o=0;o<n;o++)0!==o&&write(\", \"),s=i"\
  "[o],write(s,\": \"),e(t[s]);write(\" }\")}r.pop(t)}else\"string\"=="\
  "a?(79<(c=t.__quote()).length&&(c=c.substring(0,75)+'...\"'),wr"\
  "ite(c)):\"number\"==a?write(number_to_string(t,hex_mode?16:10))"\
  ":\"bigint\"==a?write(bigint_to_string(t,hex_mode?16:10)):\"bigfl"\
  "oat\"==a?write(bigfloat_to_string(t,hex_mode?16:10)):\"bigdecim"\
  "al\"==a?write(t.toString()+\"m\"):\"symbol\"==a?write(String(t)):\""\
  "function\"==a?write(\"function \"+t.name+\"()\"):write(t)}(e)}func"\
  "tion extract_directive(e){let r;if(\"\\\\\"!==e[0])return\"\";for(r"\
  "=1;r<e.length&&is_alpha(e[r]);r++);return e.substring(1,r)}fu"\
  "nction handle_directive(e,r){let t,n,o;if(\"h\"===e||\"?\"===e||\""\
  "help\"===e)help();else{if(\"load\"===e){let t=r.substring(e.leng"\
  "th+1).trim();return t.lastIndexOf(\".\")<=t.lastIndexOf(\"/\")&&("\
  "t+=\".js\"),0}if(\"x\"===e)hex_mode=!0;else if(\"d\"===e)hex_mode=!"\
  "1;else if(\"t\"===e)show_time=!show_time;else{if(has_bignum&&\"p"\
  "\"===e){if(1===(t=r.substring(e.length+1).trim().split(\" \")).l"\
  "ength&&\"\"===t[0])write(\"BigFloat precision=\"+prec+\" bits (~\"+"\
  "Math.floor(prec/log2_10)+\" digits), exponent size=\"+expBits+\""\
  " bits\\n\");else if(\"f16\"===t[0])prec=11,expBits=5;else if(\"f32"\
  "\"===t[0])prec=24,expBits=8;else if(\"f64\"===t[0])prec=53,expBi"\
  "ts=11;else if(\"f128\"===t[0])prec=113,expBits=15;else{if(n=par"\
  "seInt(t[0]),o=2<=t.length?parseInt(t[1]):BigFloatEnv.expBitsM"\
  "ax,Number.isNaN(n)||n<BigFloatEnv.precMin||n>BigFloatEnv.prec"\
  "Max)return write(\"Invalid precision\\n\"),0;if(Number.isNaN(o)|"\
  "|o<BigFloatEnv.expBitsMin||o>BigFloatEnv.expBitsMax)return wr"\
  "ite(\"Invalid exponent bits\\n\"),0;prec=n,expBits=o}return}if(h"\
  "as_bignum&&\"digits\"===e)return t=r.substring(e.length+1).trim"\
  "(),(n=Math.ceil(parseFloat(t)*log2_10))<BigFloatEnv.precMin||"\
  "n>BigFloatEnv.precMax?write(\"Invalid precision\\n\"):(prec=n,ex"\
  "pBits=BigFloatEnv.expBitsMax),0;if(has_bignum&&\"mode\"===e)ret"\
  "urn\"\"===(t=r.substring(e.length+1).trim())?write(\"Running mod"\
  "e=\"+eval_mode+\"\\n\"):\"std\"===t||\"math\"===t?eval_mode=t:write(\""\
  "Invalid mode\\n\"),0;if(\"clear\"===e)write(\"""\x1b""[H""\x1b""[J\");else "\
  "if(\"c\"===e)show_colors=!show_colors;else{if(\"q\"===e)return ru"\
  "nning=!1,1;if(has_jscalc&&\"a\"===e)algebraicMode=!0;else{if(!h"\
  "as_jscalc||\"n\"!==e)return write(\"Unknown directive: \"+e+\"\\n\")"\
  ",0;algebraicMode=!1}}}}return 1}function help(){function e(e)"\
  "{return e?\"*\":\" \"}write(\"\\\\h          this help\\n\\\\x         "\
  "\"+e(hex_mode)+\"hexadecimal number display\\n\\\\c          toggl"\
  "e colors\\n\\\\d         \"+e(!hex_mode)+\"decimal number display\\"\
  "n\\\\clear      clear the terminal\\n\"),has_jscalc&&write(\"\\\\a  "\
  "       \"+e(algebraicMode)+\"algebraic mode\\n\\\\n         \"+e(!a"\
  "lgebraicMode)+\"numeric mode\\n\"),has_bignum&&(write(\"\\\\p [m [e"\
  "]]  set the BigFloat precision to 'm' bits\\n\\\\digits n   set "\
  "the BigFloat precision to 'ceil(n*log2(10))' bits\\n\"),has_jsc"\
  "alc||write(\"\\\\mode [std|math] change the running mode (curren"\
  "t = \"+eval_mode+\")\\n\")),config_numcalc||write(\"\\\\q          e"\
  "xit\\n\")}function eval_and_print(expr){let result;try{\"math\"=="\
  "=eval_mode&&(expr='\"use math\"; void 0;'+expr);const now=(new "\
  "Date).getTime();result=eval(expr),eval_time=(new Date).getTim"\
  "e()-now,write(colors[styles.result]),print(result),write(\"\\n\""\
  "),write(colors.none),g._=result}catch(e){show_colors&&write(c"\
  "olors[styles.error_msg]),e instanceof Error?(console.log(e),e"\
  ".stack&&write(e.stack)):write(\"Throw: \"),show_colors&&write(c"\
  "olors.none)}}function cmd_start(){var e;config_numcalc||(e=ha"\
  "s_jscalc?\"QJSCalc\":\"QuickJS\",console.log(e,'- Type \"\\\\h\" for "\
  "help')),has_bignum&&(log2_10=Math.log(10)/Math.log(2),prec=11"\
  "3,expBits=15,has_jscalc)&&(eval_mode=\"math\",g.algebraicMode=c"\
  "onfig_numcalc),cmd_readline_start()}function cmd_readline_sta"\
  "rt(){try{readline_start(dupstr(\"    \",level),readline_handle_"\
  "cmd)}catch(e){console.error(\"ERROR\",e)}}function readline_han"\
  "dle_cmd(e){handle_cmd(e),os.pending(),cmd_readline_start()}fu"\
  "nction handle_cmd(e){if(null===e)return\"\";if(\"?\"===e||\"h\"===e"\
  ")return help();var r=extract_directive(e);if(0<r.length){if(!"\
  "handle_directive(r,e))return;e=e.substring(r.length+1)}\"\"!==e"\
  "&&(r=colorize_js(e=mexpr?mexpr+\"\\n\"+e:e),pstate=r[0],level=r["\
  "1],pstate?mexpr=e:(mexpr=\"\",has_bignum?BigFloatEnv.setPrec(ev"\
  "al_and_print.bind(null,e),prec,expBits):eval_and_print(e),lev"\
  "el=0))}function colorize_js(e){let r,t,n;const o=e.length;let"\
  " i,s=\"\",c=0,a=1;const l=[];function _(e){s+=e}function u(){re"\
  "turn s.substring(s.length-1)}function d(){var e=u();return s="\
  "s.substring(0,s.length-1),e}function f(e,r){for(;l.length<e;)"\
  "l.push(\"default\");for(;l.length<r;)l.push(i)}for(r=0;r<o;){sw"\
  "itch(i=null,n=r,t=e[r++]){case\" \":case\"\\t\":case\"\\r\":case\"\\n\":"\
  "continue;case\"+\":case\"-\":if(r<o&&e[r]===t){r++;continue}a=1;c"\
  "ontinue;case\"/\":if(r<o&&\"*\"===e[r]){for(i=\"comment\",_(\"/\"),r+"\
  "+;r<o-1;r++)if(\"*\"===e[r]&&\"/\"===e[r+1]){r+=2,d();break}break"\
  "}if(r<o&&\"/\"===e[r]){for(i=\"comment\",r++;r<o&&\"\\n\"!==e[r];r++"\
  ");break}if(a){for(i=\"regex\",_(\"/\");r<o;)if(\"\\n\"!==(t=e[r++]))"\
  "if(\"\\\\\"!==t)if(\"[\"!==u())if(\"[\"!==t){if(\"/\"===t){for(d();r<o&"\
  "&is_word(e[r]);)r++;break}}else _(\"[\"),\"[\"!==e[r]&&\"]\"!==e[r]"\
  "||r++;else\"]\"===t&&d();else r<o&&r++;else i=\"error\";a=0;break"\
  "}a=1;continue;case\"'\":case'\"':case\"`\":(function(n){for(i=\"str"\
  "ing\",_(n);r<o;)if(\"\\n\"!==(t=e[r++])){if(\"\\\\\"===t){if(r>=o)bre"\
  "ak;r++}else if(t===n){d();break}}else i=\"error\"})(t),a=0;brea"\
  "k;case\"(\":case\"[\":case\"{\":a=1,c++,_(t);continue;case\")\":case\""\
  "]\":case\"}\":if((a=0)<c&&is_balanced(u(),t)){c--,d();continue}i"\
  "=\"error\";break;default:if(is_digit(t)){for(i=\"number\";r<o&&(i"\
  "s_word(e[r])||\".\"===e[r]&&(r===o-1||\".\"!==e[r+1]));)r++;a=0}e"\
  "lse{if(!is_word(t)&&\"$\"!==t){a=1;continue}!function(){for(a=1"\
  ";r<o&&is_word(e[r]);)r++;var t=\"|\"+e.substring(n,r)+\"|\";if(0<"\
  "=\"|break|case|catch|continue|debugger|default|delete|do|else|"\
  "finally|for|function|if|in|instanceof|new|return|switch|this|"\
  "throw|try|typeof|while|with|class|const|enum|import|export|ex"\
  "tends|super|implements|interface|let|package|private|protecte"\
  "d|public|static|yield|undefined|null|true|false|Infinity|NaN|"\
  "eval|arguments|await|\".indexOf(t))return i=\"keyword\",0<=\"|thi"\
  "s|super|undefined|null|true|false|Infinity|NaN|arguments|\".in"\
  "dexOf(t)&&(a=0);let s=r;for(;s<o&&\" \"===e[s];)s++;s<o&&\"(\"==="\
  "e[s]?i=\"function\":0<=\"|void|var|\".indexOf(t)?i=\"type\":(i=\"ide"\
  "ntifier\",a=0)}()}}i&&f(n,r)}return f(o,o),[s,c,l]}config_numc"\
  "alc&&(g.execCmd=function(e){switch(e){case\"dec\":hex_mode=!1;b"\
  "reak;case\"hex\":hex_mode=!0;break;case\"num\":algebraicMode=!1;b"\
  "reak;case\"alg\":algebraicMode=!0}});try{termInit()}catch(e){co"\
  "nsole.error(e)}}(globalThis)}));\n";
