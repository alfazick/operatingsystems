class VonNeumannMachine:
    def __init__(self):
        self.memory = {}
        self.accumulator = 0
        self.program_counter = 0

    def load_program(self, program):
        for address, instruction in enumerate(program):
            self.memory[address] = instruction

    def fetch(self):
        if self.program_counter in self.memory:
            instruction = self.memory[self.program_counter]
            self.program_counter += 1
            return instruction
        else:
            return None

    def execute(self, instruction):
        parts = instruction.split()
        op = parts[0]
        arg = parts[1] if len(parts) > 1 else None

        if op == "LOAD":
            self.accumulator = int(arg)
        elif op == "ADD":
            self.accumulator += int(arg)
        elif op == "STORE":
            self.memory[int(arg)] = self.accumulator
        elif op == "PRINT":
            print(f"Output: {self.accumulator}")

    def run(self):
        while True:
            instruction = self.fetch()
            if instruction is None:
                break
            self.execute(instruction)

# Sample program: Load a value, add to it, and print the result
program = [
    "LOAD 5",
    "ADD 3",
    "STORE 10",
    "PRINT"
]

machine = VonNeumannMachine()
machine.load_program(program)
machine.run()

# Key idea: data and instructions live together

print(machine.memory)

