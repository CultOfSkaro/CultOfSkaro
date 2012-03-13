/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/******************************************************************************

FILE:    InterruptControl.h
AUTHOR:  Wade Fife
CREATED: 02/04/06

DESCRIPTION

This header file contains macros to disable and enable or restore
interrupts. It should be used as follows:

  CPU_MSR msr;

  msr = DISABLE_INTERRUPTS();
  ...
  ... // critical section here
  ...
  RESTORE_INTERRUPTS(msr);

The msr variable is used to hold the MSR register value, with the interrupt
enable bit. DISABLE_INTERRUPTS() returns the MSR register as it was before
interrupts were disabled. RESTORE_INTERRUPTS(x) restores interrupts based on
the interrupt enable bit stored in ints argument. This way, if interrupts were
already disabled when DISABLE_INTERRUPTS() is called, they will not be enabled
when RESTORE_INTERRUPTS(x) is called.

You can override this functionality by using ENABLE_INTERRUPTS(), which
unconditionally enables interrupts.
  

CHANGE LOG

02/04/06  WSF  Created initial file.

******************************************************************************/

#ifndef INTERRUPT_CONTROL_H
#define INTERRUPT_CONTROL_H

#include "Header.h"

// USER CONFIGURATION /////////////////////////////////////////////////////////

#define PPC405
// #define MICROBLAZE

// END USER CONFIGURATION /////////////////////////////////////////////////////



#ifdef PPC405

typedef uint32 CPU_MSR;

// Disables interrupts and returns the original msr value
#define DISABLE_INTERRUPTS() ({ uint32 msr;                               \
                                asm volatile ("mfmsr %0" : "=r" (msr));   \
                                asm volatile ("wrteei 0");                \
                                msr; });

// Simply enables interrupts
#define ENABLE_INTERRUPTS() asm volatile ("wrteei 1");

// Restores the interrupt value from the MSR valuex
#define RESTORE_INTERRUPTS(MSR) asm volatile ("wrtee %0" :: "r" (MSR));

#endif



#ifdef MICROBLAZE
// NOT IMPLEMENTED
#endif



#endif
