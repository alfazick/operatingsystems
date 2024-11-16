# Hardware Requirements for Dynamic Relocation

## 1. CPU Modes
- **Privileged Mode (Kernel Mode)**
  - Full machine access for OS
  - Can execute all instructions
  - Can modify system registers

- **User Mode**
  - Limited operations for applications
  - Restricted access
  - Cannot execute privileged instructions

- **Mode Control**
  - Single bit in processor status word
  - Indicates current mode
  - Switches on system calls/exceptions/interrupts

## 2. Per-CPU Memory Management Unit (MMU)
Each CPU has its own MMU containing:
- **Base Register**
  - Holds start location of process
  - Used in every address translation
  - Can only be modified in kernel mode

- **Bounds Register**
  - Contains process memory limit
  - Used for protection checks
  - Also privileged

Why per-CPU matters:
- Each CPU can run different processes
- Independent address translation
- Separate protection domains
- Enables true parallel execution

## 3. Address Translation Capabilities
Hardware must provide:
- Circuitry to add base to every virtual address
- Bounds checking for every memory access
- Fast translation as it happens on every memory reference

## 4. Privileged Instructions
1. **Base/Bounds Management**
   - Instructions to modify base/bounds registers
   - Only accessible in kernel mode
   - Used by OS for process management

2. **Exception Handler Registration**
   - Instructions to set handler locations
   - Tells hardware where to find exception code
   - Critical for protection mechanisms

## 5. Exception Generation
Hardware must raise exceptions for:
1. **Memory Violations**
   - Out-of-bounds access attempts
   - Hardware stops program
   - Transfers to OS handler

2. **Privileged Instruction Violations**
   - User attempts to modify base/bounds
   - Attempts to execute other privileged operations
   - OS handles violation, typically terminating process

## Hardware Requirements Table

| Requirement | Purpose | Notes |
|------------|---------|-------|
| Privileged mode | Protection | Prevents user processes from executing privileged operations |
| Base/bounds registers | Memory Management | Need pair per CPU for translation and bounds checks |
| Translation circuitry | Address Processing | Hardware adds base to virtual address and checks bounds |
| Privileged instructions | System Control | OS can update base/bounds and register handlers |
| Exception mechanism | Protection | Handles illegal access and privilege violations |


