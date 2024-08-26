import threading
import time

shared_resource = 0
lock = threading.Lock()

def increment():
    global shared_resource
    for _ in range(100000):
        with lock:
            shared_resource += 1
            print(f"Incremented: {shared_resource}")
        time.sleep(0.1)

def decrement():
    global shared_resource
    for _ in range(5):
        with lock:
            shared_resource -= 1
            print(f"Decremented: {shared_resource}")
        time.sleep(0.1)

thread1 = threading.Thread(target=increment)
thread2 = threading.Thread(target=decrement)

thread1.start()
thread2.start()

thread1.join()
thread2.join()

print(f"Final value: {shared_resource}")