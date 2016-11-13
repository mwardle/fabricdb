/*****************************************************************
 * FabricDB Library Property Implementation
 *
 * Copyright (c) 2016, Mark Wardle <mwwardle@gmail.com>
 *
 * This file may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 *
 ******************************************************************
 *
 * Created: July 8, 2016
 * Modified: November 12, 2016
 * Author: Mark Wardle
 * Description:
 *     Implements operations on Property and LabeledProperty objects.
 *
 ******************************************************************/

#include "property.h"
#include "byteorder.h"

void fdb_property_load(Property* prop, uint8_t* source) {
    prop->dataType = source[FDB_PROPERTY_DATATYPE_OFFSET];
    memcpy(&prop->data, source + FDB_PROPERTY_DATA_OFFSET, 8);
    prop->dataRef = NULL;
}

void fdb_property_unload(Property* prop, uint8_t* dest) {
    dest[FDB_PROPERTY_DATATYPE_OFFSET] = prop->dataType;
    memcpy(source + FDB_PROPERTY_DATA_OFFSET, &prop->data, 8);
}

uint8_t fdb_property_tobool(Property* prop) {
    switch(prop->dataType) {
        case DATATYPE_TRUE:
            return 1;
    }
    return 0;
}

int64_t fdb_property_toi64(Property* prop) {
    switch(prop->dataType) {
        case DATATYPE_INTEGER:
        case DATATYPE_DATE:
            return letohi64(*((int64_t*)prop->data));
    }
    return 0;
}

uint64_t fdb_property_tou64(Property* prop) {
    /* unsigned 64 bit integer is only used as an internal reference */
    switch(prop->dataType) {
        case DATATYPE_DOCUMENT:
        case DATATYPE_ARRAY:
        case DATATYPE_BLOB:
        case DATATYPE_STRING:
            return letohu64(*((uint64_t*)prop->data));
    }
    return 0;
}

double fdb_property_tof64(Property* prop) {
    switch(prop->dataType) {
        case DATATYPE_REAL:
            return letohf64(*((double*)prop->data));
    }
    return 0;
}

int32_t fbd_property_toi32(Property* prop) {
    /* i32 is only used for the uchar type */
    switch(prop->dataType) {
        case DATATYPE_UCHAR:
            return letohi32(*((int32_t*)prop->data));

    }
    return 0;
}

uint32_t fbd_property_tou32(Property* prop) {
    /* u32 is only used for symbol refs */
    switch(prop->dataType) {
        case DATATYPE_SYMBOL:
            return letohu32(*((uint32_t*)prop->data));
    }
    return 0;
}

ratio fdb_property_toratio(Property* prop) {
    ratio r;
    r.numer = 0;
    r.denom = 0;
    switch(prop->dataType) {
        case DATATYPE_RATIO:
            r.numer = letohu32(*((uint32_t*)prop->data));
            r.denom = letohu32(*((uint32_t*)(prop->data+4)));
    }

    return r;
}

void fdb_labeledproperty_load(LabeledProperty* prop, uint8_t* source) {
    prop->labelId = letohu32(*((uint32_t*)(source + FDB_LABELED_PROPERTY_LABELID_OFFSET)));
    fdb_property_load(&prop->prop, source + FDB_LABELED_PROPERTY_PROPERTY_OFFSET);
}

void fdb_labeledproperty_unload(LabeledProperty* prop, uint8_t* dest) {
    uint32_t labelIdOut = htoleu32(prop->labelId);
    memcpy(dest + FDB_LABELED_PROPERTY_LABELID_OFFSET, &labelIdOut);
    fdb_property_unload(&prop->prop, dest + FDB_LABELED_PROPERTY_PROPERTY_OFFSET);
}

#define fdb_property_isvoid(p) (p->dataType == DATATYPE_VOID)
#define fdb_property_isboolean(p) (p->dataType == DATATYPE_TRUE || p->dataType == DATATYPE_FALSE)
#define fdb_property_isnumeric(p) (p->dataType >= DATATYPE_INTEGER && p->dataType <= DATATYPE_DATE)
#define fdb_property_isstring(p) (p->dataType >= DATATYPE_STRING_0 && p->DATATYPE <= DATATYPE_STRING)
