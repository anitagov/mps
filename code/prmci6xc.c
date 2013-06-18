/* prmci6xc.c: PROTECTION MUTATOR CONTEXT x64 (MAC OS X)
 *
 * $Id$
 * Copyright (c) 2001-2013 Ravenbrook Limited.  See end of file for license.
 *
 * .purpose: This module implements the part of the protection module
 * that decodes the MutatorFaultContext. 
 *
 *
 * SOURCES
 *
 * .source.linux.kernel: Linux kernel source files.
 *
 *
 * ASSUMPTIONS
 *
 * .sp: The stack pointer in the context is uc_stack.ss_sp.
 *
 * .context.regroots: The root regs are assumed to be recorded in the context
 * at pointer-aligned boundaries.
 *
 * .assume.regref: The resisters in the context can be modified by
 * storing into an MRef pointer.
 */

#include "prmcix.h"
#include "prmci6.h"

SRCID(prmci6li, "$Id$");


/* Prmci6AddressHoldingReg -- return an address of a register in a context */

MRef Prmci6AddressHoldingReg(MutatorFaultContext mfc, unsigned int regnum)
{
  _STRUCT_X86_THREAD_STATE64 *ss;

  AVER(regnum <= 15);
  AVER(regnum >= 0);

  ss = &mfc->ucontext->uc_mcontext->__ss;

  /* .assume.regref */
  /* The register numbers (REG_RAX etc.) are defined in <ucontext.h>
     but only if _XOPEN_SOURCE is defined: see .feature.xc in
     config.h. */
  /* MRef (a Word *) is not compatible with pointers to the register
     types (actually a __uint64_t).  To avoid aliasing optimization
     problems, The registers are cast through (char *) */
  switch (regnum) {
    case  0: return (MRef)((char *)&ss->__rax);
    case  1: return (MRef)((char *)&ss->__rcx);
    case  2: return (MRef)((char *)&ss->__rdx);
    case  3: return (MRef)((char *)&ss->__rbx);
    case  4: return (MRef)((char *)&ss->__rsp);
    case  5: return (MRef)((char *)&ss->__rbp);
    case  6: return (MRef)((char *)&ss->__rsi);
    case  7: return (MRef)((char *)&ss->__rdi);
    case  8: return (MRef)((char *)&ss->__r8);
    case  9: return (MRef)((char *)&ss->__r9);
    case 10: return (MRef)((char *)&ss->__r10);
    case 11: return (MRef)((char *)&ss->__r11);
    case 12: return (MRef)((char *)&ss->__r12);
    case 13: return (MRef)((char *)&ss->__r13);
    case 14: return (MRef)((char *)&ss->__r14);
    case 15: return (MRef)((char *)&ss->__r15);
  }
  NOTREACHED;
  return (MRef)NULL;  /* Avoids compiler warning. */
}


/* Prmci3DecodeFaultContext -- decode fault to find faulting address and IP */

void Prmci6DecodeFaultContext(MRef *faultmemReturn,
                              Byte **insvecReturn,
                              MutatorFaultContext mfc)
{
  /* .source.linux.kernel (linux/arch/x86/mm/fault.c). */
  *faultmemReturn = (MRef)mfc->info->si_addr;
  *insvecReturn = (Byte*)mfc->ucontext->uc_mcontext->__ss.__rip;
}


/* Prmci3StepOverIns -- modify context to step over instruction */

void Prmci6StepOverIns(MutatorFaultContext mfc, Size inslen)
{
  mfc->ucontext->uc_mcontext->__ss.__rip += (Word)inslen;
}


Addr MutatorFaultContextSP(MutatorFaultContext mfc)
{
  return (Addr)mfc->ucontext->uc_mcontext->__ss.__rsp;
}


Res MutatorFaultContextScan(ScanState ss, MutatorFaultContext mfc)
{
  mcontext_t *mc;
  Res res;

  /* This scans the root registers (.context.regroots).  It also
     unnecessarily scans the rest of the context.  The optimisation
     to scan only relevant parts would be machine dependent. */
  mc = &mfc->ucontext->uc_mcontext;
  res = TraceScanAreaTagged(ss,
                            (Addr *)mc,
                            (Addr *)((char *)mc + sizeof(*mc)));
  return res;
}


/* C. COPYRIGHT AND LICENSE
 *
 * Copyright (C) 2001-2013 Ravenbrook Limited <http://www.ravenbrook.com/>.
 * All rights reserved.  This is an open source license.  Contact
 * Ravenbrook for commercial licensing options.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Redistributions in any form must be accompanied by information on how
 * to obtain complete source code for this software and any accompanying
 * software that uses this software.  The source code must either be
 * included in the distribution or be available for no more than the cost
 * of distribution plus a nominal fee, and must be freely redistributable
 * under reasonable conditions.  For an executable file, complete source
 * code means the source code for all modules it contains. It does not
 * include source code for modules or files that typically accompany the
 * major components of the operating system on which the executable file
 * runs.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
