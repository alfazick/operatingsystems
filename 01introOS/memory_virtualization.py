class VirtualMemory:
    def __init__(self, size):
        self.memory = [0] * size
        self.page_table = {}

    def allocate(self, process_id, start, size):
        for i in range(size):
            self.page_table[(process_id, start + i)] = len(self.memory) + i
        self.memory.extend([0] * size)

    def write(self, process_id, address, value):
        if (process_id, address) in self.page_table:
            physical_address = self.page_table[(process_id, address)]
            self.memory[physical_address] = value
        else:
            print(f"Memory access violation: Process {process_id}, Address {address}")

    def read(self, process_id, address):
        if (process_id, address) in self.page_table:
            physical_address = self.page_table[(process_id, address)]
            return self.memory[physical_address]
        else:
            print(f"Memory access violation: Process {process_id}, Address {address}")
            return None

vm = VirtualMemory(100)
vm.allocate(1, 0, 10)
vm.write(1, 5, 42)
print(vm.read(1, 5))  # Output: 42
print(vm.read(1, 15))  # Output: Memory access violation: Process 1, Address 15