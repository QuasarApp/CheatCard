import os

basedir = os.path.abspath(os.path.dirname(__file__))
distroDir = basedir + "/../Distro"

os.system("dpkg -i CheatCardServer.deb")
os.system("cheatcardserver -install ubuntu");

os.system("systemctl stop CheatCardServer");
os.system("systemctl start CheatCardServer");

