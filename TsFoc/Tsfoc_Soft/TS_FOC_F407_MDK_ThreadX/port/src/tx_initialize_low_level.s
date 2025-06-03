	IMPORT  _tx_thread_system_stack_ptr
	IMPORT  _tx_initialize_unused_memory
	IMPORT  _tx_thread_context_save
	IMPORT  _tx_thread_context_restore
	IMPORT  _tx_timer_interrupt
	IMPORT  __main
	IMPORT  __initial_sp
	IMPORT  __Vectors
	IMPORT  __tx_PendSVHandler
SYSTEM_CLOCK        EQU     168000000
SYSTICK_CYCLES      EQU     ((SYSTEM_CLOCK / 1000) -1)
	AREA ||.text||, CODE, READONLY
	PRESERVE8
	EXPORT  _tx_initialize_low_level
_tx_initialize_low_level
	CPSID   i
	LDR     r0, =_tx_initialize_unused_memory       ; Build address of unused memory pointer
	LDR     r1, =__initial_sp                       ; Build first free address
	ADD     r1, r1, #4                              ;
	STR     r1, [r0]                                ; Setup first unused memory pointer
	MOV     r0, #0xE000E000                         ; Build address of NVIC registers
	LDR     r1, =__Vectors                          ; Pickup address of vector table
	STR     r1, [r0, #0xD08]                        ; Set vector table address
	LDR     r0, =_tx_thread_system_stack_ptr        ; Build address of system stack pointer
	LDR     r1, =__Vectors                          ; Pickup address of vector table
	LDR     r1, [r1]                                ; Pickup reset stack pointer
	STR     r1, [r0]                                ; Save system stack pointer
	MOV     r0, #0xE000E000                         ; Build address of NVIC registers
	LDR     r1, =SYSTICK_CYCLES
	STR     r1, [r0, #0x14]                         ; Setup SysTick Reload Value
	MOV     r1, #0x7                                ; Build SysTick Control Enable Value
	STR     r1, [r0, #0x10]                         ; Setup SysTick Control
	LDR     r1, =0x00000000                         ; Rsrv, UsgF, BusF, MemM
	STR     r1, [r0, #0xD18]                        ; Setup System Handlers 4-7 Priority Registers
	LDR     r1, =0xFF000000                         ; SVCl, Rsrv, Rsrv, Rsrv
	STR     r1, [r0, #0xD1C]                        ; Setup System Handlers 8-11 Priority Registers
; Note: SVC must be lowest priority, which is 0xFF
	LDR     r1, =0x40FF0000                     ; SysT, PnSV, Rsrv, DbgM
	STR     r1, [r0, #0xD20]                    ; Setup System Handlers 12-15 Priority Registers
	BX      lr
	EXPORT  SysTick_Handler
SysTick_Handler
	PUSH    {r0, lr}
	BL      _tx_timer_interrupt
	POP     {r0, lr}
	BX      LR
	ALIGN
LTORG
	END