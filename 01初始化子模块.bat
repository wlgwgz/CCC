@echo off

::���ã������ģ�飬ָ��main��֧��ǿ��
::git submodule add -b main --force -- "git@192.168.0.251:hvac-group/hvac-cbb/cbb-exio-klv.git" "Code/User/APP/CTRL/cbb-exio-klv"

echo ͬ����ģ��URL
git submodule sync

echo ����������ģ��
git submodule update --init --force

echo.
echo ϡ������ģ��: cbb-exio-klv 

:: ����ģ����Ϊ"ϡ����"
git -C Code/User/APP/CTRL/cbb-exio-klv config core.sparseCheckout true
 
:: �����Ҫ�õ����ض��ļ�/�ļ���
echo README.md > .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo exio_klv.h >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo exio_klv.c >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo core/exio_klv_core.h >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo core/exio_klv_core.c >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo core/0/* >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo slp/nt_dlg/exio_klv_nt_dlg.h >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo slp/nt_dlg/exio_klv_nt_dlg.c >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout
echo slp/nt_dlg/0/* >> .git/modules/Code/User/APP/CTRL/cbb-exio-klv/info/sparse-checkout

echo. 
echo ���¸���������ģ��
git submodule update --init --force

echo. 
echo ������ģ�飺cbb-exio-klv����ȡ��ǩv0.0.1
cd Code/User/APP/CTRL/cbb-exio-klv
git fetch
git checkout tags/v0.0.1
git pull
cd ..