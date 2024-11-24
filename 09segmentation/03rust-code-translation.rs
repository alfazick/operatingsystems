struct SegmentRegister {
    base: usize,
    limit: usize,
    grows_down: bool, // Direction of growth
}

struct MMU {
    code_segment: SegmentRegister,
    heap_segment: SegmentRegister,
    stack_segment: SegmentRegister,
    offset_bits: usize,
}

impl MMU {
    fn new() -> Self {
        let offset_bits = 12; // Number of bits for the offset
        
        MMU {
            offset_bits,
            code_segment: SegmentRegister {
                base: 32_768,    // 32KB
                limit: 2_048,    // 2KB
                grows_down: false,
            },
            heap_segment: SegmentRegister {
                base: 34_816,    // 34KB
                limit: 3_072,    // 3KB
                grows_down: false,
            },
            stack_segment: SegmentRegister {
                base: 28_672,    // 28KB
                limit: 2_048,    // 2KB
                grows_down: true,
            },
        }
    }

    fn translate(&self, virtual_addr: usize) -> Result<usize, &str> {
        let offset_mask = (1 << self.offset_bits) - 1; // Mask for the offset bits
        let segment = virtual_addr >> self.offset_bits; // Extract segment bits
        let raw_offset = virtual_addr & offset_mask; // Extract raw offset

        println!("\nDetailed translation for virtual address: {}", virtual_addr);
        println!("Address bits: {:014b}", virtual_addr);
        println!("Segment bits: {:02b}", segment);
        println!("Raw offset bits: {:012b}", raw_offset);
        println!("Raw offset decimal: {}", raw_offset);

        let segment_reg = match segment {
            0 => {
                println!("Segment type: Code (00)");
                &self.code_segment
            },
            1 => {
                println!("Segment type: Heap (01)");
                &self.heap_segment
            },
            3 => {
                println!("Segment type: Stack (11)");
                &self.stack_segment
            },
            _ => return Err("Invalid segment"),
        };

        println!("Selected segment base: {}", segment_reg.base);
        println!("Segment limit: {}", segment_reg.limit);

        // Calculate effective offset based on growth direction
        let effective_offset = if segment_reg.grows_down {
            let max_offset = 1 << self.offset_bits; // Maximum offset value (e.g., 4096)
            if raw_offset > max_offset {
                return Err("Segmentation violation - raw offset exceeds virtual space");
            }
            max_offset - raw_offset
        } else {
            raw_offset
        };

        println!("Calculated offset: {}", effective_offset);

        // Check if the calculated offset exceeds the segment limit
        if effective_offset > segment_reg.limit {
            return Err("Segmentation violation - offset exceeds segment limit");
        }

        // Calculate physical address
        let physical_addr = if segment_reg.grows_down {
            println!("\nStack translation (grows down):");
            println!("Physical = Base({}) - Offset({})", segment_reg.base, effective_offset);
            segment_reg.base - effective_offset
        } else {
            println!("\nRegular translation (grows up):");
            println!("Physical = Base({}) + Offset({})", segment_reg.base, effective_offset);
            segment_reg.base + effective_offset
        };

        Ok(physical_addr)
    }
}

fn main() {
    let mmu = MMU::new();

    println!("SEGMENTATION TRANSLATION EXAMPLES");
    println!("--------------------------------");
    println!("Virtual address space: 14-bit (16384 bytes)");
    println!("Segment bits: 2 (top bits)");
    println!("Offset bits: 12 (bottom bits)\n");

    let addresses = vec![
        4200,   // Heap example
        14400,  // Stack example
    ];

    for &addr in &addresses {
        match mmu.translate(addr) {
            Ok(phys_addr) => {
                println!("Final physical address: {}", phys_addr);
                println!("Translation successful!");
            }
            Err(e) => println!("Translation error: {}", e),
        }
    }
}
