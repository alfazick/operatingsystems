# Understanding Virtual Address Translation Bit Operations

Let's break it down step by step. This code is critical for interpreting the virtual address by splitting it into segment bits and offset bits:

## Code Explanation

### 1. Creating the Offset Mask
```rust
let offset_mask = (1 << self.offset_bits) - 1; // Mask for the offset bits
```

What it does: Creates a bitmask that isolates the lower offset_bits of the virtual address.

How it works:
- `1 << self.offset_bits`: Shifts the binary 1 to the left by self.offset_bits positions. If self.offset_bits = 12, this creates a number with only the 13th bit set: 0b1000000000000 (decimal 4096).
- Subtracting 1 (`- 1`): Turns this into a bitmask where the lower self.offset_bits bits are all 1. For 12 bits, this results in 0b111111111111 (decimal 4095).

Why it's important: This mask can be applied to any number using a bitwise AND operation to extract only the lower 12 bits (the offset).

Example:
For self.offset_bits = 12:
```bash
offset_mask = (1 << 12) - 1
           = 4096 - 1
           = 0b111111111111
```

### 2. Extracting Segment Bits
```rust
let segment = virtual_addr >> self.offset_bits; // Extract segment bits
```

What it does: Extracts the segment bits from the virtual address.

How it works:
- `>> self.offset_bits`: Right-shifts the virtual_addr by self.offset_bits positions. This effectively discards the lower self.offset_bits (offset bits), leaving only the higher-order bits (segment bits).

Why it's important: The segment bits determine which segment (Code, Heap, Stack) the virtual address belongs to.

Example:
For a 14-bit virtual address 0b01000001101000 (decimal 4200):
```makefile
virtual_addr = 0b01000001101000
segment = virtual_addr >> 12
        = 0b01 (decimal 1)
```
This identifies the address as belonging to the Heap segment (segment 01).

### 3. Extracting Raw Offset
```rust
let raw_offset = virtual_addr & offset_mask; // Extract raw offset
```

What it does: Extracts the offset within the segment from the virtual address.

How it works:
- `& offset_mask`: Performs a bitwise AND operation between virtual_addr and offset_mask. This clears all higher-order bits (segment bits), leaving only the lower self.offset_bits bits (the offset).

Why it's important: The offset tells us the position within the selected segment.

Example:
Using the same virtual address (0b01000001101000):
```csharp
offset_mask = 0b111111111111 (decimal 4095)
raw_offset = virtual_addr & offset_mask
           = 0b01000001101000 & 0b111111111111
           = 0b0000001101000 (decimal 104)
```
This extracts the offset 104 within the Heap segment.

## Combined Workflow

For a 14-bit virtual address:

1. Extract the Segment Bits:
   - Right-shift by offset_bits to get the segment ID.

2. Extract the Offset Bits:
   - Use a bitmask to isolate the lower offset_bits.

3. Interpretation:
   - The segment ID determines which segment's base and limit to use.
   - The offset specifies the location within the segment.

Example Workflow for Virtual Address 4200:

- Virtual address (binary): 0b01000001101000
- Offset mask: 0b111111111111
- Segment bits: 0b01 (Heap segment).
- Raw offset: 0b0000001101000 (decimal 104).

This logic is central to segment-based memory management, where virtual addresses are split into segments and offsets for efficient translation.

