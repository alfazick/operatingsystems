class SimpleFileSystem:
    def __init__(self):
        self.files = {}
        self.directories = {"root": {}}

    def create_file(self, path, content=""):
        dir_path, filename = path.rsplit("/", 1)
        if dir_path in self.directories:
            self.files[path] = content
            self.directories[dir_path][filename] = path
            print(f"File created: {path}")
        else:
            print(f"Directory not found: {dir_path}")

    def read_file(self, path):
        if path in self.files:
            return self.files[path]
        else:
            print(f"File not found: {path}")
            return None

    def create_directory(self, path):
        self.directories[path] = {}
        print(f"Directory created: {path}")

    def list_directory(self, path):
        if path in self.directories:
            return list(self.directories[path].keys())
        else:
            print(f"Directory not found: {path}")
            return None

fs = SimpleFileSystem()
fs.create_directory("root/documents")
fs.create_file("root/documents/note.txt", "Hello, World!")
print(fs.read_file("root/documents/note.txt"))
print(fs.list_directory("root/documents"))