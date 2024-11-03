import json
import os
import sys

def find_vscode_settings_path(start_dir):
    current_dir = start_dir
    while current_dir != os.path.dirname(current_dir):  # Stop at the root directory
        vscode_settings_path = os.path.join(current_dir, '.vscode', 'settings.json')
        if os.path.exists(vscode_settings_path):
            return vscode_settings_path
        current_dir = os.path.dirname(current_dir)
    return None

def merge_settings():
    active_file_dir = sys.argv[1]
    vscode_settings_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.json')
    custom_settings_path = find_vscode_settings_path(active_file_dir)

    if not custom_settings_path:
        print('settings.json file does not exist in the .vscode directory. Exiting.')
        return

    try:
        with open(custom_settings_path, 'r') as f:
            custom_settings = json.load(f)

        if os.path.abspath(custom_settings_path) == os.path.abspath(vscode_settings_path):
            vscode_settings = custom_settings
        else:
            with open(vscode_settings_path, 'r') as f_vscode:
                vscode_settings = json.load(f_vscode)

        if 'C_ARGS_BASE' not in custom_settings['tasks']:
            print('C_ARGS_BASE is not defined in custom_settings. Skipping merging.')
        else:
            vscode_settings['tasks']['C_ARGS_BASE'] = custom_settings['tasks']['C_ARGS_BASE']
            vscode_settings['tasks']['c_args_base'] = ' '.join(custom_settings['tasks']['C_ARGS_BASE'])

        if 'C_ARGS' not in custom_settings['tasks']:
            print('C_ARGS is not defined in custom_settings. Skipping merging.')
        else:
            vscode_settings['tasks']['C_ARGS'] = custom_settings['tasks']['C_ARGS']
            vscode_settings['tasks']['c_args'] = ' '.join(custom_settings['tasks']['C_ARGS'])

        if 'CPP_ARGS_BASE' not in custom_settings['tasks']:
            print('CPP_ARGS_BASE is not defined in custom_settings. Skipping merging.')
        else:
            vscode_settings['tasks']['CPP_ARGS_BASE'] = custom_settings['tasks']['CPP_ARGS_BASE']
            vscode_settings['tasks']['cpp_args_base'] = ' '.join(custom_settings['tasks']['CPP_ARGS_BASE'])

        if 'CPP_ARGS' not in custom_settings['tasks']:
            print('CPP_ARGS is not defined in custom_settings. Skipping merging.')
        else:
            vscode_settings['tasks']['CPP_ARGS'] = custom_settings['tasks']['CPP_ARGS']
            vscode_settings['tasks']['cpp_args'] = ' '.join(custom_settings['tasks']['CPP_ARGS'])

        # Ensure the new keys exist in the settings
        if 'c_args_base' not in vscode_settings['tasks']:
            vscode_settings['tasks']['c_args_base'] = ' '.join(custom_settings.get('C_ARGS_BASE', []))
        if 'c_args' not in vscode_settings['tasks']:
            vscode_settings['tasks']['c_args'] = ' '.join(custom_settings.get('C_ARGS', []))
        if 'cpp_args_base' not in vscode_settings['tasks']:
            vscode_settings['tasks']['cpp_args_base'] = ' '.join(custom_settings.get('CPP_ARGS_BASE', []))
        if 'cpp_args' not in vscode_settings['tasks']:
            vscode_settings['tasks']['cpp_args'] = ' '.join(custom_settings.get('CPP_ARGS', []))

        with open(vscode_settings_path, 'w') as f_vscode:
            json.dump(vscode_settings, f_vscode, indent=2)
            print('Settings merged successfully!')

    except FileNotFoundError:
        print('settings.json file does not exist in the custom_settings_path. Exiting.')

if __name__ == "__main__":
    merge_settings()
