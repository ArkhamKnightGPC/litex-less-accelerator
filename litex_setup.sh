python -m venv venv
source ./venv/bin/activate
pip3 install meson
pip3 install pyvcd
chmod +x litex_setup.py
./litex_setup.py --init --install --config=standard --tag=2025.08
