/*
 * This file is part of OpenPLC Runtime
 *
 * Copyright (C) 2023 Autonomy, GP Orcullo
 * Based on the work by GP Orcullo on Beremiz for uC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdbool.h>

#include "iec_types_all.h"
#include "POUS.h"

extern BLINK RES0__INSTANCE0;

static const struct {
    void *ptr;
    __IEC_types_enum type;
} debug_vars[] = {
    {&(RES0__INSTANCE0.BLINK_LED), BOOL_O_ENUM},
    {&(RES0__INSTANCE0.TON0.EN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TON0.ENO), BOOL_ENUM},
    {&(RES0__INSTANCE0.TON0.IN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TON0.PT), TIME_ENUM},
    {&(RES0__INSTANCE0.TON0.Q), BOOL_ENUM},
    {&(RES0__INSTANCE0.TON0.ET), TIME_ENUM},
    {&(RES0__INSTANCE0.TON0.STATE), SINT_ENUM},
    {&(RES0__INSTANCE0.TON0.PREV_IN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TON0.CURRENT_TIME), TIME_ENUM},
    {&(RES0__INSTANCE0.TON0.START_TIME), TIME_ENUM},
    {&(RES0__INSTANCE0.TOF0.EN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TOF0.ENO), BOOL_ENUM},
    {&(RES0__INSTANCE0.TOF0.IN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TOF0.PT), TIME_ENUM},
    {&(RES0__INSTANCE0.TOF0.Q), BOOL_ENUM},
    {&(RES0__INSTANCE0.TOF0.ET), TIME_ENUM},
    {&(RES0__INSTANCE0.TOF0.STATE), SINT_ENUM},
    {&(RES0__INSTANCE0.TOF0.PREV_IN), BOOL_ENUM},
    {&(RES0__INSTANCE0.TOF0.CURRENT_TIME), TIME_ENUM},
    {&(RES0__INSTANCE0.TOF0.START_TIME), TIME_ENUM},
};

#define VAR_COUNT               21

uint16_t get_var_count(void)
{
    return VAR_COUNT;
}

size_t get_var_size(size_t idx)
{
    switch (debug_vars[idx].type) {
    case SINT_ENUM:
        return sizeof(SINT);
    case BOOL_ENUM:
    case BOOL_O_ENUM:
        return sizeof(BOOL);
    case TIME_ENUM:
        return sizeof(TIME);
    default:
        return 0;
    }
}

void *get_var_addr(size_t idx)
{
    void *ptr = debug_vars[idx].ptr;

    switch (debug_vars[idx].type) {
    case SINT_ENUM:
        return (void *)&((__IEC_SINT_t *) ptr)->value;
    case BOOL_ENUM:
        return (void *)&((__IEC_BOOL_t *) ptr)->value;
    case BOOL_O_ENUM:
        return (void *)((((__IEC_BOOL_p *) ptr)->flags & __IEC_FORCE_FLAG) 
                        ? &(((__IEC_BOOL_p *) ptr)->fvalue) 
                        : ((__IEC_BOOL_p *) ptr)->value);
    case TIME_ENUM:
        return (void *)&((__IEC_TIME_t *) ptr)->value;
    default:
        return 0;
    }
}

void force_var(size_t idx, bool forced, void *val)
{
    void *ptr = debug_vars[idx].ptr;

    if (forced) {
        switch (debug_vars[idx].type) {
        case SINT_ENUM:
            ((__IEC_SINT_t *) ptr)->value = *((SINT *) val);
            ((__IEC_SINT_t *) ptr)->flags |= __IEC_FORCE_FLAG;
            break;
        case BOOL_ENUM:
            ((__IEC_BOOL_t *) ptr)->value = *((BOOL *) val);
            ((__IEC_BOOL_t *) ptr)->flags |= __IEC_FORCE_FLAG;
            break;
        case BOOL_O_ENUM:
            *(((__IEC_BOOL_p *) ptr)->value) = *((BOOL *) val);
            ((__IEC_BOOL_p *) ptr)->flags |= __IEC_FORCE_FLAG;
            break;
        case TIME_ENUM:
            ((__IEC_TIME_t *) ptr)->value = *((TIME *) val);
            ((__IEC_TIME_t *) ptr)->flags |= __IEC_FORCE_FLAG;
            break;
        default:
            break;
        }
    } else {
        switch (debug_vars[idx].type) {
        case SINT_ENUM:
            ((__IEC_SINT_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;
            break;
        case BOOL_ENUM:
            ((__IEC_BOOL_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;
            break;
        case BOOL_O_ENUM:
            ((__IEC_BOOL_p *) ptr)->flags &= ~__IEC_FORCE_FLAG;
            break;
        case TIME_ENUM:
            ((__IEC_TIME_t *) ptr)->flags &= ~__IEC_FORCE_FLAG;
            break;
        default:
            break;
        }
    }
}

void trace_reset(void)
{
    for (size_t i=0; i < VAR_COUNT; i++) {
        force_var(i, false, 0);
    }
}

void set_trace(size_t idx, bool forced, void *val)
{
    if (idx >= 0 && idx < VAR_COUNT) {
        force_var(idx, forced, val);
    }
}
