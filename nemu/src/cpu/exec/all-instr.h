#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

#include "template/call.h"
#include "template/ret.h"
#include "template/push.h"
#include "template/pop.h"
#include "template/cmp.h"
#include "template/test.h"
#include "template/add.h"
#include "template/adc.h"
#include "template/sub.h"
#include "template/sbb.h"
#include "template/setcc.h"
#include "template/jmp.h"
#include "template/jcc.h"
#include "template/movs.h"
#include "template/stos.h"
#include "template/lods.h"
#include "template/scas.h"
#include "template/leave.h"