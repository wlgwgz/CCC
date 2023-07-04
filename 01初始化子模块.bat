@echo off

::弃用：添加子模块，指定main分支，强制
::git submodule add -b main --force -- "git@192.168.0.251:hvac-group/hvac-cbb/cbb-exio-klv.git" "Code/User/APP/CTRL/cbb-exio-klv"

echo 同步子模块URL
git submodule sync

echo 更新所有子模块
git submodule update --init --force

echo.
echo 稀疏检出子模块: cbb-exio-klv 

:: 将子模块设为"稀疏检出"
git -C Code/User/APP/CTRL/cbb-exio-klv config core.sparseCheckout true
 
:: 检出需要用到的特定文件/文件夹
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
echo 重新更新所有子模块
git submodule update --init --force

echo. 
echo 进入子模块：cbb-exio-klv，拉取标签v0.0.1
cd Code/User/APP/CTRL/cbb-exio-klv
git fetch
git checkout tags/v0.0.1
git pull
cd ..