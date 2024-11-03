import os
import shutil

def restore_settings():
    backup_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.backup.json')
    restore_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.json')
    try:
        shutil.copy(backup_path, restore_path)
        print("Settings restored successfully.")
        os.remove(backup_path)
        print("Backup file deleted.")
    except FileNotFoundError:
        print("Except: settings.backup.json not found.")
        default_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.default.json')
        shutil.copy(default_path, restore_path)
        print("Settings restored successfully.")

if __name__ == "__main__":
    restore_settings()
