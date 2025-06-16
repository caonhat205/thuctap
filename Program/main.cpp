#include "NhanVien.h"
#include "menu.h"
#include"ChucNang.h"

int main() {
	manHinhKhoiDong();
    vector<NhanVien> ds;
    docFile(ds, "nhanvien.dat");
    system("chcp 65001");
    while (true) {
        int chon = menuChon({
            "1. Them moi ho so (M1)",
            "2. In danh sach (M2)",
            "3. Sap xep (M3)",
            "4. Tim kiem (M4)",
            "5. Thong ke (M5)",
            "6. Thoat (M6)"
        }, "======= MENU CHINH =======");

	switch (chon) {
	    case 0: themMoiNhanVien(ds); break;
	    case 1: inDanhSach(ds); break;
	    case 2: sapXep(ds); break;
	    case 3: timKiem(ds); break;
	    case 4: thongKe(ds); break;
	    case 5: 
	        ghiFile(ds, "nhanvien.dat");
	        return 0;
	    default: break;
	}
	    }
    return 0;
}