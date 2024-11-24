# Address Translation in Segmentation: Heap and Stack Examples

```mermaid
graph TD
    subgraph "16KB Virtual Address Space"
        VA[16KB Total Space] -->
        Code["Code: 0-2KB<br/>(0-2048 bytes)"]
        VA --> 
        Gap1["Unused: 2KB-4KB<br/>(2048-4096 bytes)"]
        VA --> 
        Heap["Heap: 4KB-7KB<br/>(4096-7168 bytes)"]
        VA --> 
        Free["Free: 7KB-14KB<br/>(7168-14336 bytes)"]
        VA --> 
        Stack["Stack: 14KB-16KB<br/>(14336-16384 bytes)"]
    end
```

## 1. Basic Translation Structure

```mermaid
graph TD
    subgraph "Virtual Address Format"
        VA[14-bit Virtual Address] --> SB[Segment Bits<br/>2 bits]
        VA --> OFF[Offset<br/>12 bits]
        SB --> |00| Code[Code Segment]
        SB --> |01| Heap[Heap Segment]
        SB --> |11| Stack[Stack Segment]
    end
```

## 2. Heap Address Translation Example

Let's walk through translating virtual address 4200 (heap access):

```mermaid
graph TD
    subgraph "Heap Translation"
        VA4200[Virtual Address: 4200<br/>0x1068] --> 
        Binary["Binary: 01 0000 0110 1000"]
        Binary --> SegBits["Segment Bits: 01<br/>(Heap)"]
        Binary --> Offset["Offset: 0000 0110 1000<br/>(104 bytes)"]
        
        SegBits --> HeapBase["Heap Base: 34KB<br/>(0x8800)"]
        Offset --> Add["Add to Base"]
        HeapBase --> Add
        Add --> Physical["Physical Address:<br/>34KB + 104 = 34920<br/>(0x8868)"]
    end
```

Calculation Steps:
1. Virtual Address 4200 (0x1068)
2. Top 2 bits (01) indicate heap segment
3. Remove segment start (4200 - 4096 = 104)
4. Add to heap base (34KB + 104 = 34920)

## 3. Stack Address Translation

Stack is special because it grows downward. Let's translate address 15KB (stack access):

```mermaid
graph TD
    subgraph "Stack Translation"
        VA15K[Virtual Address: 15KB<br/>0x3C00] -->
        Binary2["Binary: 11 1100 0000 0000"]
        Binary2 --> SegBits2["Segment Bits: 11<br/>(Stack)"]
        Binary2 --> Offset2["Offset: 1100 0000 0000<br/>(3KB)"]
        
        SegBits2 --> StackBase["Stack Base: 28KB"]
        Offset2 --> Subtract["Calculate Negative Offset<br/>4KB - 3KB = 1KB"]
        
        StackBase --> AddNeg["Add Negative Offset"]
        Subtract --> AddNeg
        AddNeg --> Physical2["Physical Address:<br/>28KB - 1KB = 27KB"]
    end
```

Stack Translation Steps:
1. Virtual Address 15KB (0x3C00)
2. Top 2 bits (11) indicate stack segment
3. Calculate negative offset from max size
4. Subtract from stack base

## 4. Bounds Checking

```mermaid
graph TD
    subgraph "Bounds Checking Process"
        VA[Virtual Address] --> Extract[Extract Offset]
        Extract --> Compare{Compare with Limit}
        Compare --> |"Offset <= Limit"| Valid[Valid Access<br/>Continue Translation]
        Compare --> |"Offset > Limit"| Fault[Segmentation Fault]
        
        Valid --> Growth{Check Growth Direction}
        Growth --> |Positive| Forward[Add to Base]
        Growth --> |Negative| Backward[Subtract from Base]
    end
```

Example Segment Register Values:
```
Segment   Base    Limit    Growth    Virtual Range
Heap      34KB    3KB     Forward   4KB - 7KB
Stack     28KB    2KB     Backward  14KB - 16KB
```

## 5. Translation Examples

### Heap Access Example (4200):
```
Virtual Address:  4200 (0x1068)
Segment Bits:     01 (Heap)
Offset:           104 bytes
Physical Address: Base + Offset
                 34KB + 104 = 34920
Bounds Check:     104 < 3KB (valid)
```

### Stack Access Example (15KB):
```
Virtual Address:  15KB (0x3C00)
Segment Bits:     11 (Stack)
Original Offset:  3KB
Negative Offset:  1KB (4KB - 3KB)
Physical Address: Base - Negative Offset
                 28KB - 1KB = 27KB
Bounds Check:     1KB < 2KB (valid)
```

## 6. Common Translation Errors

```mermaid
graph TD
    subgraph "Common Translation Failures"
        VA[Virtual Address] --> Check1{Valid Segment Bits?}
        Check1 --> |No| Err1[Invalid Segment]
        Check1 --> |Yes| Check2{Within Bounds?}
        Check2 --> |No| Err2[Segmentation Fault]
        Check2 --> |Yes| Check3{Valid Permissions?}
        Check3 --> |No| Err3[Protection Fault]
        Check3 --> |Yes| Success[Successful Translation]
    end
```

Common errors include:
- Accessing beyond segment limits
- Using invalid segment numbers
- Violating segment permissions
- Not accounting for growth direction
