import os
import zipfile


def zip_dir(src_dir, dst_name, new_root_name=None, ignore_dir=['__pycache__']):
    zipf = zipfile.ZipFile(dst_name, 'w', zipfile.ZIP_DEFLATED)
    for root, dirs, files in os.walk(src_dir):
        if not files:
            continue
        if os.path.basename(root) in ignore_dir:
            continue
        for file in files:
            arcname = os.path.join(os.path.relpath(root, src_dir), file)
            if new_root_name:
                arcname = os.path.join(new_root_name, arcname)
            zipf.write(os.path.join(root, file), arcname)
    zipf.close()


if __name__ == '__main__':
    zip_dir('/tmp/dir', '/tmp/dir.zip')
