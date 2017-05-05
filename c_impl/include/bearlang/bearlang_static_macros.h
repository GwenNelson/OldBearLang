#pragma once

// this file includes useful macros for declaring values statically

#define BL_STATIC_SYM(_SymName)         {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_SYMBOL,      .sym_name=_SymName}
#define BL_STATIC_NATIVEFUNC(_FuncCode) {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_FUNC_NATIVE, .fn_native_code=_FuncCode}
#define BL_STATIC_NATIVEOPER(_FuncCode) {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_OPER_NATIVE, .fn_native_code=_FuncCode}
#define BL_STATIC_CONS(_Car,_Cdr)       {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_CONS,        .car=_Car, .cdr=_Cdr}

#define BL_STATIC_ASSOC_ENTRY(_AssocListName,_EntryNum) BL_STATIC_CONS(&( _AssocListName ## _keys [_EntryNum]),&( _AssocListName ## _vals [_EntryNum]))
