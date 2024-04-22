set SDKDRIVE=c:
set SDK=\sdk-java.201
md default
copy *.class default
%SDKDRIVE%%SDK%\bin\packsign\dubuild @colors.du

%SDKDRIVE%%SDK%\bin\packsign\makecert -sk Colors -n "CN=Microsoft Speech SDK" Colors.cer
%SDKDRIVE%%SDK%\bin\packsign\cert2spc Colors.cer Colors.spc

%SDKDRIVE%%SDK%\bin\packsign\signcode -k Colors -spc Colors.spc -i "http://microsoft.com" -j %SDKDRIVE%%SDK%\bin\packsign\javasign.dll -jp LowX Colors.cab
