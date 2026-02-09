python -m venv venv
source ./venv/bin/activate
pip3 install meson
chmod +x litex_setup.py
./litex_setup.py --init --install --config=standard --tag=2025.08
