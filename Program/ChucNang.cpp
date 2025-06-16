#include"ChucNang.h"
#include"menu.h"
#include <iomanip>
#include <map>

bool cmpNgaySinh(const Date& a, const Date& b) {
    if (a.year != b.year) return a.year < b.year;
    if (a.month != b.month) return a.month < b.month;
    return a.day < b.day;
}
string layTen(const string& hoTen) {
    size_t pos = hoTen.find_last_of(' ');
    if (pos != string::npos) {
        return hoTen.substr(pos + 1);
    }
    return hoTen;
}

string layHo(const string& hoTen) {
    size_t pos = hoTen.find(' ');
    if (pos != string::npos) {
        return hoTen.substr(0, pos);
    }
    return hoTen;
}

bool soSanhMaNhanVien(const NhanVien& a, const NhanVien& b){
    return a.maNhanVien < b.maNhanVien;
}

bool soSanhMaPB(const NhanVien& a, const NhanVien& b) {
    return a.maPhongBan < b.maPhongBan;
}

bool soSanhHoTen(const NhanVien& a, const NhanVien& b) {
    string tenA = layTen(a.hoTen);
    string tenB = layTen(b.hoTen);
    if (tenA != tenB) return tenA < tenB;
    return a.hoTen < b.hoTen;
}

bool soSanhNgaySinh(const NhanVien& a, const NhanVien& b) {
    if (a.ngaySinh.year != b.ngaySinh.year) 
        return a.ngaySinh.year < b.ngaySinh.year;
    if (a.ngaySinh.month != b.ngaySinh.month) 
        return a.ngaySinh.month < b.ngaySinh.month;
    return a.ngaySinh.day < b.ngaySinh.day;
}
bool soSanhLuong(const NhanVien& a, const NhanVien& b){
    return a.luong < b.luong;
}

// So sánh động nhiều tiêu chí
bool SoSanhNhieuTieuChi::operator()(const NhanVien& a, const NhanVien& b) const {
    for (int idx : tieuChi) {
        switch (idx) {
            case 0: // Ma phong ban
                if (a.maPhongBan != b.maPhongBan)
                    return a.maPhongBan < b.maPhongBan;
                break;
            case 1: // Ho ten
                if (a.hoTen != b.hoTen)
                    return a.hoTen < b.hoTen;
                break;
            case 2: // Ngay sinh
                if (a.ngaySinh.year != b.ngaySinh.year)
                    return a.ngaySinh.year < b.ngaySinh.year;
                if (a.ngaySinh.month != b.ngaySinh.month)
                    return a.ngaySinh.month < b.ngaySinh.month;
                if (a.ngaySinh.day != b.ngaySinh.day)
                    return a.ngaySinh.day < b.ngaySinh.day;
                break;
            case 3: // Ma nhan vien
                if (a.maNhanVien != b.maNhanVien)
                    return a.maNhanVien < b.maNhanVien;
                break;
            case 4: // Luong
                if (a.luong != b.luong)
                    return a.luong < b.luong;
                break;
        }
    }
    return false;
}

// Selection Sort
void selectionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    for (size_t i = 0; i < ds.size() - 1; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < ds.size(); ++j) {
            if (cmp(ds[j], ds[minIdx])) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(ds[i], ds[minIdx]);
        }
    }
}

// Insertion Sort
void insertionSort(vector<NhanVien>& ds, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    for (size_t i = 1; i < ds.size(); ++i) {
        NhanVien key = ds[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, ds[j])) {
            ds[j + 1] = ds[j];
            j--;
        }
        ds[j + 1] = key;
    }
}

// Quick Sort
int partition(vector<NhanVien>& ds, int low, int high, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    NhanVien pivot = ds[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (cmp(ds[j], pivot)) {
            i++;
            swap(ds[i], ds[j]);
        }
    }
    swap(ds[i + 1], ds[high]);
    return i + 1;
}

void quickSort(vector<NhanVien>& ds, int low, int high, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    if (low < high) {
        int pi = partition(ds, low, high, cmp);
        quickSort(ds, low, pi - 1, cmp);
        quickSort(ds, pi + 1, high, cmp);
    }
}

// Merge Sort
void merge(vector<NhanVien>& ds, int left, int mid, int right, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<NhanVien> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
        L[i] = ds[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = ds[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp(L[i], R[j])) {
            ds[k] = L[i];
            i++;
        } else {
            ds[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        ds[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        ds[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(vector<NhanVien>& ds, int left, int right, const function<bool(const NhanVien&, const NhanVien&)>& cmp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(ds, left, mid, cmp);
        mergeSort(ds, mid + 1, right, cmp);
        merge(ds, left, mid, right, cmp);
    }
}

// Sắp xếp tổng quát
void sapXep(vector<NhanVien> ds) {
    int algo = menuChon({
        "Selection Sort",
        "Insertion Sort",
        "Quick Sort",
        "Merge Sort"
    }, "=== CHON THUAT TOAN SAP XEP ===");

    int khoa = menuChon({
        "Ma phong ban",
        "Ho ten",
        "Ngay sinh",
        "Ma nhan vien",
        "Luong",
        "Nhieu tieu chi"
    }, "=== CHON KHOA SAP XEP ===");

    function<bool(const NhanVien&, const NhanVien&)> cmp;
    vector<int> chonTieuChi;

    if (khoa == 5) {
        vector<string> cacTieuChi = {
            "Ma phong ban",
            "Ho ten",
            "Ngay sinh",
            "Ma nhan vien",
            "Luong"
        };
        vector<bool> daChon(cacTieuChi.size(), false);

        for (int i = 0; i < 5; ++i) {
            vector<string> menuTieuChi;
            for (int j = 0; j < cacTieuChi.size(); ++j)
                if (!daChon[j]) menuTieuChi.push_back(cacTieuChi[j]);
            int chon = menuChon(menuTieuChi, "Chon tieu chi sap xep thu " + to_string(i + 1));
            if (chon < 0) break;
            int idx = 0, cnt = -1;
            for (; idx < cacTieuChi.size(); ++idx)
                if (!daChon[idx] && ++cnt == chon) break;
            chonTieuChi.push_back(idx);
            daChon[idx] = true;
        }
        SoSanhNhieuTieuChi soSanh{chonTieuChi};
        cmp = soSanh;
    } else {
        switch (khoa) {
            case 0: cmp = soSanhMaPB; break;
            case 1: cmp = soSanhHoTen; break;
            case 2: cmp = soSanhNgaySinh; break;
            case 3: cmp = soSanhMaNhanVien; break;
            case 4: cmp = soSanhLuong; break;
        }
    }

    // Gọi thuật toán đã chọn
    switch (algo) {
        case 0: selectionSort(ds, cmp); break;
        case 1: insertionSort(ds, cmp); break;
        case 2: quickSort(ds, 0, ds.size() - 1, cmp); break;
        case 3: mergeSort(ds, 0, ds.size() - 1, cmp); break;
    }
    string note = "Dang sap xep theo: ";
    if (khoa == 0) note += "Ma phong ban";
    else if (khoa == 1) note += "Ho ten";
    else if (khoa == 2) note += "Ngay sinh";
    else if (khoa == 3) note += "Ma nhan vien";
    else if (khoa == 4) note += "Luong";
    else if (khoa == 5) note += "Nhieu tieu chi (";
    if (khoa == 5) {
        for (int i = 0; i < chonTieuChi.size(); ++i) {
            if (chonTieuChi[i] == 0) note += "Ma phong ban";
            else if (chonTieuChi[i] == 1) note += "Ho ten";
            else if (chonTieuChi[i] == 2) note += "Ngay sinh";
            else if (chonTieuChi[i] == 3) note += "Ma nhan vien";
            else if (chonTieuChi[i] == 4) note += "Luong";
            if (i < chonTieuChi.size() - 1) note += " -> ";
        }
        note += ")";
    }

    clearScreen();
    inDanhSach(ds, note);
}

vector<NhanVien> timKiemChinhXac(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = false;
        switch (khoa) {
            case 0: // Ma phong ban
                match = (nv.maPhongBan == inHoaChuoi(giaTriChuan));
                break;
            case 1: // Ma nhan vien
                match = (nv.maNhanVien == giaTriChuan);
                break;
            case 2: // Ho ten
                match = (nv.hoTen == chuanHoaTen(giaTriChuan));
                break;
            case 3: { // Ngay sinh
                stringstream ss(giaTriChuan);
                int day, month, year;
                if (ss >> day >> month >> year) {
                    match = (nv.ngaySinh.day == day && 
                            nv.ngaySinh.month == month && 
                            nv.ngaySinh.year == year);
                }
                break;
            }
            case 4: { // Luong
                try {
                    double luong = stod(giaTriChuan);
                    match = (nv.luong == luong);
                } catch (...) {}
                break;
            }
        }
        if (match) {
            ketQua.push_back(nv);
        }
    }
    return ketQua;
}

vector<NhanVien> timKiemGanDung(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = false;
        switch (khoa) {
            case 0: // Ma phong ban
                match = (nv.maPhongBan.find(inHoaChuoi(giaTriChuan)) != string::npos);
                break;
            case 1: // Ma nhan vien
                match = (nv.maNhanVien.find(giaTriChuan) != string::npos);
                break;
            case 2: // Ho ten
                match = (nv.hoTen.find(chuanHoaTen(giaTriChuan)) != string::npos);
                break;
            case 3: { // Ngay sinh
                stringstream ss;
                ss << setfill('0') << setw(2) << nv.ngaySinh.day << " "
                   << setfill('0') << setw(2) << nv.ngaySinh.month << " "
                   << setw(4) << nv.ngaySinh.year;
                string ngaySinhStr = ss.str();
                match = (ngaySinhStr.find(giaTriChuan) != string::npos);
                break;
            }
            case 4: { // Luong
                string luongStr = to_string(nv.luong);
                match = (luongStr.find(giaTriChuan) != string::npos);
                break;
            }
        }
        if (match) {
            ketQua.push_back(nv);
        }
    }
    return ketQua;
}

vector<NhanVien> timKiemToanBo(const vector<NhanVien>& ds, const string& giaTri) {
    vector<NhanVien> ketQua;
    if (ds.empty()) return ketQua;

    string giaTriChuan = trim(giaTri);
    for (const auto& nv : ds) {
        bool match = (nv.maPhongBan.find(giaTriChuan) != string::npos ||
                     nv.tenPhongBan.find(giaTriChuan) != string::npos ||
                     nv.maNhanVien.find(giaTriChuan) != string::npos ||
                     nv.hoTen.find(giaTriChuan) != string::npos ||
                     nv.chucVu.find(giaTriChuan) != string::npos);
        
        // Kiểm tra ngày sinh
        if (!match) {
            stringstream ss;
            ss << setfill('0') << setw(2) << nv.ngaySinh.day << " "
               << setfill('0') << setw(2) << nv.ngaySinh.month << " "
               << setw(4) << nv.ngaySinh.year;
            string ngaySinhStr = ss.str();
            match = (ngaySinhStr.find(giaTriChuan) != string::npos);
        }
        
        // Kiểm tra lương
        if (!match) {
            string luongStr = to_string(nv.luong);
            match = (luongStr.find(giaTriChuan) != string::npos);
        }
        
        if (match) {
            ketQua.push_back(nv);
        }
    }
    return ketQua;
}

vector<NhanVien> timKiemTuanTu(const vector<NhanVien>& ds, int khoa, const string& giaTri) {
    return timKiemChinhXac(ds, khoa, giaTri);
}
vector<NhanVien> timKiemLuongTheoKhoang(const vector<NhanVien>& ds, double tu, double den) {
    if (tu > den) swap(tu, den);
    vector<NhanVien> ketQua;
    for (const auto& nv : ds) {
        if (nv.luong >= tu && nv.luong <= den) ketQua.push_back(nv);
    }
    return ketQua;
}

vector<NhanVien> timKiemNgaySinhTheoKhoang(const vector<NhanVien>& ds, const Date& tu, const Date& den) {
    vector<NhanVien> ketQua;
    for (const auto& nv : ds) {
        if (!cmpNgaySinh(nv.ngaySinh, tu) && !cmpNgaySinh(den, nv.ngaySinh))
            ketQua.push_back(nv);
    }
    return ketQua;
}

struct DieuKienTimKiem {
    string maPhongBan;
    string maNhanVien;
    string hoTen;
    double luongTu = 0, luongDen = 0;
    Date ngayTu, ngayDen;
    bool locLuong = false, locNgaySinh = false;
};

DieuKienTimKiem nhapDieuKienTimKiem() {
    DieuKienTimKiem dk;
    vector<string> options = {"PB001","PB002","PB003","PB004","PB005"};
    int x = 10, y = 5, w = 50;
    string temp;

    clearScreen();
    setColor(11);
    drawBox(x, y, w, 12, 11);
    setColor(14);
    gotoxy(x+2, y+1); cout << "NHAP DIEU KIEN TIM KIEM (Ko can bo qua):";
    setColor(7);

    // Mã phòng ban - combobox
    gotoxy(x+2, y+2); cout << "Ma phong ban: ";
    dk.maPhongBan = chonOptionCombobox(x+55, y+2, options);
    xoaVungConsole(x+55,y,x+100,y+8);
    gotoxy(x+2, y+2);
    setColor(10);
    cout << "Ma phong ban da chon: " <<dk.maPhongBan;

    // Mã nhân viên
    gotoxy(x+2, y+3); cout << "Ma nhan vien: ";
    getline(cin, temp);
    dk.maNhanVien = trim(temp);

    // Họ tên
    gotoxy(x+2, y+4); cout << "Ho ten: ";
    getline(cin, temp);
    dk.hoTen = trim(temp);

    // Lương (textbox: từ - đến)
    gotoxy(x+2, y+5); cout << "Luong tu: ";
    getline(cin, temp);
    if (!trim(temp).empty()) {
        dk.luongTu = stod(temp);
        dk.locLuong = true;
    }
    gotoxy(x+2, y+6); cout << "Luong den: ";
    getline(cin, temp);
    if (!trim(temp).empty()) {
        dk.luongDen = stod(temp);
        dk.locLuong = true;
    }

    // Ngày sinh (textbox: từ - đến)
    gotoxy(x+2, y+7); cout << "Ngay sinh tu (dd mm yyyy): ";
    getline(cin, temp);
    if (!trim(temp).empty()) {
        sscanf(temp.c_str(), "%d %d %d", &dk.ngayTu.day, &dk.ngayTu.month, &dk.ngayTu.year);
        dk.locNgaySinh = true;
    }
    gotoxy(x+2, y+8); cout << "Ngay sinh den (dd mm yyyy): ";
    getline(cin, temp);
    if (!trim(temp).empty()) {
        sscanf(temp.c_str(), "%d %d %d", &dk.ngayDen.day, &dk.ngayDen.month, &dk.ngayDen.year);
        dk.locNgaySinh = true;
    }

    setColor(7);
    return dk;
}
vector<NhanVien> timKiemNhieuTruong(const vector<NhanVien>& ds, const DieuKienTimKiem& dk) {
    vector<NhanVien> kq;
    for (const auto& nv : ds) {
        if (!dk.maPhongBan.empty() && nv.maPhongBan != dk.maPhongBan) continue;
        if (!dk.maNhanVien.empty() && nv.maNhanVien.find(dk.maNhanVien) == string::npos) continue;
        if (!dk.hoTen.empty() && nv.hoTen.find(dk.hoTen) == string::npos) continue;
        if (dk.locLuong) {
            if (dk.luongTu && nv.luong < dk.luongTu) continue;
            if (dk.luongDen && nv.luong > dk.luongDen) continue;
        }
        if (dk.locNgaySinh) {
            if (dk.ngayTu.day && cmpNgaySinh(nv.ngaySinh, dk.ngayTu)) continue;
            if (dk.ngayDen.day && cmpNgaySinh(dk.ngayDen, nv.ngaySinh)) continue;
        }
        kq.push_back(nv);
    }
    return kq;
}

void nhapTuKhoaBox(int x, int y, string &result, const string &label, int boxWidth = 40) {
    setColor(11); // màu xanh lam nhạt
    drawBox(x, y, boxWidth, 3, 11);
    gotoxy(x + 2, y + 1);
    setColor(14); // vàng nhạt cho label
    cout << label;
    setColor(7); // màu thường cho nhập liệu
    getline(cin, result);
    setColor(7);
}

void nhapKhoangLuongBox(int x, int y, double &tu, double &den, int boxWidth = 40) {
    setColor(11);
    drawBox(x, y, boxWidth, 5, 11);
    gotoxy(x + 2, y + 1);
    setColor(14);
    cout << "Nhap khoang luong:";
    gotoxy(x + 2, y + 2);
    setColor(7);
    cout << "- Tu: "; cin >> tu;
    gotoxy(x + 2, y + 3);
    cout << "- Den: "; cin >> den;
    cin.ignore();
    setColor(7);
}

void nhapNgaySinhBox(int x, int y, Date &tu, Date &den, int boxWidth = 40) {
    setColor(11);
    drawBox(x, y, boxWidth, 5, 11);
    gotoxy(x + 2, y + 1);
    setColor(14);
    cout << "Nhap ngay sinh bat dau (dd mm yyyy):";
    gotoxy(x + 2, y + 2);
    setColor(7);
    cin >> tu.day >> tu.month >> tu.year;
    gotoxy(x + 2, y + 3);
    setColor(14);
    cout << "Nhap ngay sinh ket thuc (dd mm yyyy):";
    gotoxy(x + 2, y + 4);
    setColor(7);
    cin >> den.day >> den.month >> den.year;
    cin.ignore();
    setColor(7);
}

void timKiem(const vector<NhanVien>& ds) {
    int mode = menuChon({
        "Tim kiem chinh xac",
        "Tim kiem gan dung",
        "Tim kiem toan bo",
        "Tim kiem nhieu truong"
    }, "=== CHON PHUONG PHAP TIM KIEM ===");
    if (mode == -1) return;

    vector<NhanVien> ketQua;
    string note;
    string giaTri;
    double luongTu = 0, luongDen = 0;
    Date ngayTu, ngayDen;
    vector<string> options = {"PB001","PB002","PB003","PB004","PB005"};
    int xInput = 10, yInput = 10;

    if (mode == 3) {
        DieuKienTimKiem dk = nhapDieuKienTimKiem();
        ketQua = timKiemNhieuTruong(ds, dk);

        note = "KQ tim kiem nhieu truong: ";
        if (!dk.maPhongBan.empty()) note += "[MaPhongBan: " + dk.maPhongBan + "] ";
        if (!dk.maNhanVien.empty()) note += "[MaNV: " + dk.maNhanVien + "] ";
        if (!dk.hoTen.empty()) note += "[HoTen: " + dk.hoTen + "] ";
        if (dk.locLuong) {
            note += "[Luong: ";
            note += (dk.luongTu ? to_string((int)dk.luongTu) : "-");
            note += " ~ ";
            note += (dk.luongDen ? to_string((int)dk.luongDen) : "-");
            note += "] ";
        }
        if (dk.locNgaySinh) {
            note += "[NgaySinh: ";
            if (dk.ngayTu.day) note += to_string(dk.ngayTu.day)+"/"+to_string(dk.ngayTu.month)+"/"+to_string(dk.ngayTu.year);
            else note += "-";
            note += " ~ ";
            if (dk.ngayDen.day) note += to_string(dk.ngayDen.day)+"/"+to_string(dk.ngayDen.month)+"/"+to_string(dk.ngayDen.year);
            else note += "-";
            note += "] ";
        }

        clearScreen();
        if (ketQua.empty()) {
            setColor(12);
            gotoxy(xInput, yInput);
            cout << "Khong tim thay ket qua nao!";
            setColor(7);
        } else {
            inDanhSach(ketQua, note);
        }
        _getwch();
        return;
    }

    int field = menuChon({
        "Ma phong ban",
        "Ma nhan vien",
        "Ho ten",
        "Luong",
        "Ngay sinh"
    }, "=== CHON TRUONG TIM KIEM ===");
    if (field == -1) return;

    clearScreen();

    if (field == 0) {
        setColor(11);
        drawBox(xInput, yInput, 44, 4, 11);
        gotoxy(xInput + 2, yInput + 1);
        setColor(14);
        cout << "Chon ma phong ban:";
        giaTri = chonOptionCombobox(xInput + 2, yInput + 2, options);
        setColor(7);
        note = "KQ tim theo MaPhongBan: " + giaTri;
    } else if (field == 1) {
        nhapTuKhoaBox(xInput, yInput, giaTri, "Nhap ma nhan vien can tim:");
        note = "KQ tim theo MaNV: " + giaTri;
    } else if (field == 2) {
        nhapTuKhoaBox(xInput, yInput, giaTri, "Nhap ho ten can tim:");
        note = "KQ tim theo HoTen: " + giaTri;
    } else if (field == 3) {
        nhapKhoangLuongBox(xInput, yInput, luongTu, luongDen);
        note = "KQ tim theo khoang Luong: " + to_string((int)luongTu) + " ~ " + to_string((int)luongDen);
    } else if (field == 4) {
        nhapNgaySinhBox(xInput, yInput, ngayTu, ngayDen);
        note = "KQ tim theo khoang NgaySinh: " +
            to_string(ngayTu.day) + "/" + to_string(ngayTu.month) + "/" + to_string(ngayTu.year)
            + " ~ " +
            to_string(ngayDen.day) + "/" + to_string(ngayDen.month) + "/" + to_string(ngayDen.year);
    }

    switch (field) {
        case 0:
            ketQua = (mode == 0) ? timKiemChinhXac(ds, 0, giaTri)
                    : (mode == 1) ? timKiemGanDung(ds, 0, giaTri)
                    : timKiemToanBo(ds, giaTri);
            break;
        case 1:
            ketQua = (mode == 0) ? timKiemChinhXac(ds, 1, giaTri)
                    : (mode == 1) ? timKiemGanDung(ds, 1, giaTri)
                    : timKiemToanBo(ds, giaTri);
            break;
        case 2:
            ketQua = (mode == 0) ? timKiemChinhXac(ds, 2, giaTri)
                    : (mode == 1) ? timKiemGanDung(ds, 2, giaTri)
                    : timKiemToanBo(ds, giaTri);
            break;
        case 3:
            ketQua = timKiemLuongTheoKhoang(ds, luongTu, luongDen);
            break;
        case 4:
            ketQua = timKiemNgaySinhTheoKhoang(ds, ngayTu, ngayDen);
            break;
    }

    clearScreen();
    if (ketQua.empty()) {
        setColor(12);
        gotoxy(xInput, yInput);
        cout << "Khong tim thay ket qua nao!";
        setColor(7);
    } else {
        inDanhSach(ketQua, note, giaTri);
    }
}

void veBieuDo(const map<string, int>& data, const string& tieuDe, const string& trucX, int startX, int startY) {
    if (data.empty()) return;

    const int maxHeight = 12;
    const int colWidth = 5;
    const int paddingLeft = 3;
    const int paddingRight = 3;
    const int borderColor = 11, titleColor = 14, axisColor = 8, labelColor = 7, valueColor = 14, gridColor = 8;

    // Chuẩn bị dữ liệu
    vector<string> labels;
    vector<int> values;
    int maxVal = 0;
    for (const auto& item : data) {
        labels.push_back(item.first);
        values.push_back(item.second);
        if (item.second > maxVal) maxVal = item.second;
    }
    if (maxVal == 0) maxVal = 1;

    // Tính độ dài nhãn dài nhất
    size_t maxLabelLen = 0;
    for (const auto& lbl : labels) maxLabelLen = max(maxLabelLen, lbl.length());

    // Đặt colGap tự động để không chồng nhãn
    int colGap = max(4, (int)maxLabelLen - colWidth + 2);

    int yAxisWidth = max((int)to_string(maxVal).length(), 2) + 1;
    int chartWidth = labels.size() * (colWidth + colGap) - colGap;
    int totalWidth = yAxisWidth + chartWidth + paddingLeft + paddingRight + 1;

    // Tính tổng chiều cao khung ngoài đủ chứa tất cả phần
    int labelRow = 1, belowValueRow = 1, trucXRow = trucX.empty() ? 0 : 1;
    int topValueRow = 1; // dòng cho giá trị nằm trên cột
    int bottomPadding = 1;
    int khungHeight = topValueRow + maxHeight + 1 + labelRow + belowValueRow + trucXRow + bottomPadding;

    // Tiêu đề
    setColor(titleColor);
    gotoxy(startX + (totalWidth - tieuDe.length()) / 2 + 1, startY - 1);
    cout << tieuDe;

    // Lưới ngang + trục Y
    for (int h = 0; h <= maxHeight; ++h) {
        int currVal = maxVal * (maxHeight - h) / maxHeight;
        gotoxy(startX + 1, startY + h + 1 + topValueRow);
        setColor(axisColor);
        cout << setw(yAxisWidth) << right << currVal << " ";

        // Lưới ngang
        setColor(gridColor);
        cout << char(179);
        cout << string(paddingLeft, ' ');
        for (int x = 0; x < chartWidth; x++) {
            cout << (h % 2 == 0 ? char(196) : ' ');
        }
        cout << string(paddingRight, ' ');
    }

    // CỘT và GIÁ TRỊ TRÊN ĐỈNH CỘT
    for (size_t i = 0; i < values.size(); ++i) {
        int barHeight = values[i] * maxHeight / maxVal;
        int cX = startX + 1 + yAxisWidth + paddingLeft + i * (colWidth + colGap);
        // Giá trị trên đầu cột
        string valStr = to_string(values[i]);
        int valX = cX + (colWidth - valStr.length()) / 2;
        int valY = startY + topValueRow;
        setColor(valueColor);
        gotoxy(valX, valY);
        cout << valStr;

        // Vẽ cột
        setColor(10 + i % 6);
        for (int y = 0; y < barHeight; y++) {
            for (int w = 0; w < colWidth; w++) {
                gotoxy(cX + w, startY + topValueRow + maxHeight - y);
                cout << char(219);
            }
        }
    }

    // Trục X
    gotoxy(startX + 1 + yAxisWidth + paddingLeft, startY + topValueRow + maxHeight + 1);
    setColor(axisColor);
    cout << "+";
    for (size_t i = 0; i < labels.size(); ++i) {
        cout << string(colWidth, char(196));
        if (i < labels.size() - 1) cout << string(colGap, ' ');
    }

    // Nhãn cột (căn giữa CHUẨN)
    setColor(labelColor);
    for (size_t i = 0; i < labels.size(); ++i) {
        const string& lbl = labels[i];
        int cX = startX + 1 + yAxisWidth + paddingLeft + i * (colWidth + colGap) +
                 (colWidth + colGap - lbl.length()) / 2;
        gotoxy(cX, startY + topValueRow + maxHeight + 2);
        cout << lbl;
    }
    
    if (!trucX.empty()) {
        gotoxy(startX + yAxisWidth + paddingLeft + (chartWidth - trucX.length()) / 2 + 1, 
               startY + topValueRow + maxHeight + 3);
        setColor(labelColor);
        cout << trucX;
    }

    setColor(7);
    cout<<"\n\n\n\n";
}

void thongKe(const vector<NhanVien>& ds) {
    if(ds.empty()) {
        cout << "Danh sach nhan vien trong!\n";
        system("pause");
        return;
    }
    int luaChon = menuChon({
        "So luong NV theo phong ban",
        "Ty le NV theo muc luong",
        "So luong NV theo nam sinh",
        "So luong NV theo chuc vu"
    }, "=== THONG KE ===");
    clearScreen();

    int startX = 10, startY = 8;

    if (luaChon == 0) {
        map<string, int> phongBan;
        for (const auto& nv : ds) {
            phongBan[nv.maPhongBan]++;
        }
        cout << left << setw(25) << "Ma Phong Ban" << setw(25) << "Ten Phong Ban" << "So Luong NV\n";
        cout << string(65, '-') << "\n";

        map<string, string> danhSachPhong;
        ifstream file("PhongBan.txt");
        if (!file.is_open()) {
            cout << "Khong the mo file PhongBan.txt\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            if (pos != string::npos) {
                string ma = line.substr(0, pos);
                string ten = line.substr(pos + 1);
                danhSachPhong[ma] = ten;
            }
        }
        file.close();

        for (const auto& p : phongBan) {
            string tenPhong = danhSachPhong.count(p.first) ? danhSachPhong[p.first] : "Khong xac dinh";
            cout << left << setw(25) << p.first << setw(25) << tenPhong << p.second << "\n";
        }
        cout << "\n";
        veBieuDo(phongBan, "Bieu do so luong nhan vien theo phong ban", "Ma Phong Ban", startX, startY + 5 + phongBan.size());
    } else if (luaChon == 1) {
        int duoi5 = 0, tu5den10 = 0, tren10 = 0;
        for (const auto& nv : ds) {
            if (nv.luong < 5000) duoi5++;
            else if (nv.luong <= 10000) tu5den10++;
            else tren10++;
        }
        int tong = ds.size();
        cout << left << setw(25) << "Muc Luong" << setw(15) << "So Luong NV" << "Ty Le (%)\n";
        cout << string(56, '-') << "\n";
        cout << left << setw(25) << "Duoi 5tr" << setw(15) << duoi5 << fixed << setprecision(2) << (100.0 * duoi5 / tong) << "%\n";
        cout << left << setw(25) << "5tr-10tr" << setw(15) << tu5den10 << fixed << setprecision(2) << (100.0 * tu5den10 / tong) << "%\n";
        cout << left << setw(25) << "Tren 10tr" << setw(15) << tren10 << fixed << setprecision(2) << (100.0 * tren10 / tong) << "%\n";
        cout << "Tong so NV: " << tong << "\n\n";
        map<string, int> thongKeLuong;
        thongKeLuong["Duoi 5tr"] = duoi5;
        thongKeLuong["5tr-10tr"] = tu5den10;
        thongKeLuong["Tren 10tr"] = tren10;
        veBieuDo(thongKeLuong, "Bieu do so luong nhan vien theo muc luong", "Muc Luong", startX, startY + 8);
    } else if (luaChon == 2) {
        map<string, int> thongKeNamSinh;
        for (const auto& nv : ds) {
            thongKeNamSinh[to_string(nv.ngaySinh.year)]++;
        }
        cout << left << setw(15) << "Nam Sinh" << "So Luong NV\n";
        cout << string(25, '-') << "\n";
        for (const auto& t : thongKeNamSinh) {
            cout << left << setw(15) << t.first << t.second << "\n";
        }
        cout << "\n";
        veBieuDo(thongKeNamSinh, "Bieu do so luong nhan vien theo nam sinh", "Nam sinh", startX, startY + 5 + thongKeNamSinh.size());
    } else if (luaChon == 3) {
        map<string, int> thongKeChucVu;
        for (const auto& nv : ds) {
            thongKeChucVu[nv.chucVu]++;
        }
        cout << left << setw(25) << "Chuc Vu" << "So Luong NV\n";
        cout << string(45, '-') << "\n";
        for (const auto& t : thongKeChucVu) {
            cout << left << setw(25) << t.first << t.second << "\n";
        }
        cout << "\n";
        veBieuDo(thongKeChucVu, "Bieu do so luong nhan vien theo chuc vu", "Chuc vu", startX, startY + 5 + thongKeChucVu.size());
    }

    system("pause");
}