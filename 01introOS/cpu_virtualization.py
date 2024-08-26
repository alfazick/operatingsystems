import time

class Process:
    def __init__(self, pid, name, duration):
        self.pid = pid
        self.name = name
        self.duration = duration

class SimpleCPUScheduler:
    def __init__(self, time_slice=0.060):  # 60ms time slice
        self.processes = []
        self.time_slice = time_slice

    def add_process(self, process):
        self.processes.append(process)

    def run(self):
        while self.processes:
            current_process = self.processes.pop(0)
            print(f"Running process {current_process.name} for {self.time_slice*1000}ms")
            time.sleep(self.time_slice)
            
            current_process.duration -= self.time_slice
            if current_process.duration > 0:
                self.processes.append(current_process)
            else:
                print(f"Process {current_process.name} completed")
            
            print("System interrupt: switching process")

scheduler = SimpleCPUScheduler()
scheduler.add_process(Process(1, "P1", 0.15))
scheduler.add_process(Process(2, "P2", 0.10))
scheduler.run()