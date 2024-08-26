class SimpleFileSystem:
    def __init__(self):
        self.files = {}

    def write_file(self, filename, content):
        self.files[filename] = content
        print(f"File '{filename}' written to disk")

    def read_file(self, filename):
        if filename in self.files:
            print(f"Reading file '{filename}' from disk")
            return self.files[filename]
        else:
            print(f"File '{filename}' not found")
            return None

    def delete_file(self, filename):
        if filename in self.files:
            del self.files[filename]
            print(f"File '{filename}' deleted from disk")
        else:
            print(f"File '{filename}' not found")

fs = SimpleFileSystem()
fs.write_file("example.txt", "Hello, World!")
print(fs.read_file("example.txt"))
fs.delete_file("example.txt")