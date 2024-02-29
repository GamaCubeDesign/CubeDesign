cd "/home/bcr/rasp/vitinho/TTEC-Gamacubedesign/main/Satellite"
sudo ./SatTTC.out & disown
cd "/home/bcr/rasp/vitinho/TTEC-Gamacubedesign/main/Satellite/obc/"
sudo ./sensor_runner.sh & disown
cd "/home/bcr/rasp/vitinho/TTEC-Gamacubedesign/main/Satellite/ImagingClient/opencv/"
sudo ./main & disown