import hashlib

class SimpleSecuritySystem:
    def __init__(self):
        self.users = {}

    def create_user(self, username, password):
        hashed_password = self._hash_password(password)
        self.users[username] = hashed_password
        print(f"User '{username}' created")

    def authenticate(self, username, password):
        if username in self.users:
            hashed_password = self._hash_password(password)
            if self.users[username] == hashed_password:
                print(f"User '{username}' authenticated successfully")
                return True
        print("Authentication failed")
        return False

    def _hash_password(self, password):
        return hashlib.sha256(password.encode()).hexdigest()

security_system = SimpleSecuritySystem()
security_system.create_user("alice", "password123")
security_system.authenticate("alice", "password123")  # Successful
security_system.authenticate("alice", "wrong_password")  # Failed