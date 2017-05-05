#pragma once

// this file includes useful macros for declaring values statically

#define BL_STATIC_SYM(_SymName)         {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_SYMBOL,      .sym_name=_SymName}
#define BL_STATIC_NATIVEFUNC(_FuncCode) {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_FUNC_NATIVE, .fn_native_code=_FuncCode}
#define BL_STATIC_NATIVEOPER(_FuncCode) {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_OPER_NATIVE, .fn_native_code=_FuncCode}
#define BL_STATIC_CONS(_Car,_Cdr)       {.alloc_type = VAL_ALLOC_STATIC, .type=VAL_TYPE_CONS,        .car=_Car, .cdr=_Cdr}

#define BL_STATIC_ASSOC_KEY(_KeyName) BL_STATIC_SYM(_KeyName)
#define BL_STATIC_ASSOC_VAL(_ValName) BL_STATIC_CONS( &( _ValName), NULL)
#define BL_STATIC_ASSOC_ITEM(_AssocListName,_EntryNum) BL_STATIC_CONS(&( _AssocListName ## _items_keys [_EntryNum]),&( _AssocListName ## _items_vals [_EntryNum]))

#define BL_STATIC_LIST_CONS(_ListName,_EntryNum) BL_STATIC_CONS(&( _ListName ## _items[_EntryNum]),&( _ListName ## _list [_EntryNum +1]))
#define BL_STATIC_LIST_CONS_LAST(_ListName, _EntryNum) BL_STATIC_CONS(&( _ListName ## _items[_EntryNum]),NULL)

#define BL_ASSOC_VAL_START(AssocName) static bl_val_t AssocName##_items_vals[]; \
        static bl_val_t AssocName##_items_vals[] = {

#define BL_ASSOC_VAL_ENTRY(_AssocKey,_AssocVal,_AssocName,_AssocI) BL_STATIC_ASSOC_VAL(_AssocVal),

#define BL_ASSOC_VAL_END };

#define BL_ASSOC_KEY_START(AssocName) static bl_val_t AssocName##_items_keys[]; \
        static bl_val_t AssocName##_items_keys[] = {

#define BL_ASSOC_KEY_ENTRY(_AssocKey,_AssocVal,_AssocName,_AssocI) BL_STATIC_ASSOC_KEY(_AssocKey),

#define BL_ASSOC_KEY_END };

#define BL_ASSOC_ITEMS_START(AssocName) static bl_val_t AssocName##_items[]; \
        static bl_val_t AssocName##_items[] = {

#define BL_ASSOC_ITEMS_ENTRY(_AssocKey,_AssocVal,_AssocName,_AssocI) BL_STATIC_ASSOC_ITEM(_AssocName,_AssocI),

#define BL_ASSOC_ITEMS_END };
