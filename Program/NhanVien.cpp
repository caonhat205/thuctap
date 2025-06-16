#include "NhanVien.h"
#include"ChuanHoa.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <map>
using namespace std;

string nhapVaChinhSuaTrenConsole(const vector<string>& label, vector<string>& data, 
                                const vector<bool>& invalidFields, const vector<NhanVien>& ds) {
    const int formWidth = 70;
    const int fieldHeight = 3;
    const int startX = 25;
    const int startY = 4;
    int cur = 0;
    bool firstDraw = true;
    int comboBoxSelection = 0;

    // Đọc danh sách phòng ban từ file
    map<string, string> danhSachPhong;
    ifstream file("PhongBan.txt");
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

    vector<string> dsMaPhong, dsTenPhong;
    for (const auto& pb : danhSachPhong) {
        dsMaPhong.push_back(pb.first);
        dsTenPhong.push_back(pb.second);
    }

    if (invalidFields.size() == label.size()) {
        for (int i = 0; i < invalidFields.size(); ++i) {
            if (invalidFields[i]) {
                cur = i;
                break;
            }
        }
    }

    int idxMaPB = -1, idxTenPB = -1;
    for (int i = 0; i < label.size(); ++i) {
        if (label[i] == "Ma phong ban") idxMaPB = i;
        if (label[i] == "Ten phong ban") idxTenPB = i;
    }

    vector<int> inputPositions(label.size());
    int labelWidth = 24;
    for (int i = 0; i < label.size(); ++i) {
        inputPositions[i] = startX + 4 + labelWidth + 2;
    }

    auto drawFormBox = [&](int height) {
        setColor(8);
        gotoxy(startX, startY - 2);
        cout << char(201) << string(formWidth - 2, char(205)) << char(187);
        for (int i = 0; i < height; ++i) {
            gotoxy(startX, startY - 1 + i);
            cout << char(186) << string(formWidth - 2, ' ') << char(186);
        }
        gotoxy(startX, startY - 1 + height);
        cout << char(200) << string(formWidth - 2, char(205)) << char(188);
        setColor(7);
    };

    auto drawField = [&](int i, bool isSelected) {
        int y = startY + i * fieldHeight;

        int fieldBoxX = startX + 2, fieldBoxW = formWidth - 4;
        setColor(isSelected ? 11 : 7);
        // Viền mỗi trường
        gotoxy(fieldBoxX, y);
        cout << char(218) << string(fieldBoxW - 2, char(196)) << char(191);
        gotoxy(fieldBoxX, y + 1);
        cout << char(179) << string(fieldBoxW - 2, ' ') << char(179);
        gotoxy(fieldBoxX, y + 2);
        cout << char(192) << string(fieldBoxW - 2, char(196)) << char(217);

        // Label căn trái, gọn
        gotoxy(fieldBoxX + 2, y + 1);
        cout << left << setw(labelWidth) << label[i] << ": ";

        // Data
        string showData = data[i].substr(0, fieldBoxW - (labelWidth + 8));
        cout << showData;

        // Nếu là trường mã phòng ban và đang chọn thì hiện combobox
        if (i == idxMaPB && isSelected) {
            vector<string> options;
            for (int k = 0; k < dsMaPhong.size(); ++k)
                options.push_back(dsMaPhong[k] + " - " + dsTenPhong[k]);
            veCombobox(fieldBoxX + fieldBoxW + 2, y, options, comboBoxSelection);
        }
        setColor(7);
    };

    auto drawAll = [&]() {
        int formHeight = label.size() * fieldHeight + 5;
        clearScreen();
        // Khung ngoài cho form
        drawFormBox(formHeight);

        // Tiêu đề
        setColor(10);
        gotoxy(startX + (formWidth - 22) / 2, startY - 2);
        cout << "NHAP/CHINH SUA NHAN VIEN";
        setColor(8);
        gotoxy(startX + 2, startY - 1);
        cout << "DUNG MUI TEN DE DI CHUYEN | ENTER DE CHON | ESC DE HUY";
        setColor(7);

        for (int i = 0; i < label.size(); ++i) {
            drawField(i, i == cur);
        }

        // Vẽ dòng lỗi
        if (invalidFields.size() == label.size()) {
            setColor(12);
            gotoxy(startX + 2, startY + label.size() * fieldHeight + 1);
            cout << "THONG BAO LOI:";
            int line = 2;
            for (int i = 0; i < invalidFields.size(); ++i) {
                if (invalidFields[i]) {
                    gotoxy(startX + 4, startY + label.size() * fieldHeight + line);
                    if (data[i].empty()) {
                        cout << "- " << label[i] << " khong duoc de trong!";
                        line++;
                    } else if (label[i] == "Ngay sinh (dd mm yyyy)") {
                        cout << "- Ngay sinh khong hop le hoac nhan vien chua du 18 tuoi!";
                        line++;
                    } else if (label[i] == "Luong (nghin VND)") {
                        try {
                            stod(data[i]);
                            cout << "- Luong khong hop le (phai >= 0)!";
                        } catch (const std::invalid_argument&) {
                            cout << "- Luong phai la so hop le!";
                        } catch (const std::out_of_range&) {
                            cout << "- Luong qua lon!";
                        }
                        line++;
                    } else if (label[i] == "Ma nhan vien (8 chu so)") {
                        if (data[i].empty()) {
                            cout << "- Ma nhan vien khong duoc de trong!";
                        } else if (isDuplicateID(ds, trim(data[i]))) {
                            cout << "- Ma nhan vien da ton tai!";
                        } else {
                            cout << "- Ma nhan vien khong hop le (8 ky tu, chi bao gom chu va so)!";
                        }
                        line++;
                    } else {
                        cout << "- " << label[i] << " khong hop le!";
                        line++;
                    }
                }
            }
        }
        setColor(7);
    };

    // Field bỏ qua
    auto nextField = [&](int cur, int dir) {
        int n = label.size();
        int newCur = cur;
        do {
            newCur = (newCur + dir + n) % n;
        } while (newCur == idxTenPB); // Bỏ qua trường tên phòng ban
        return newCur;
    };

    // Tự động điền tên phòng ban khi mã phòng ban thay đổi
    auto autoFillTenPhongBan = [&]() {
        if (idxMaPB != -1 && idxTenPB != -1) {
            string ma = data[idxMaPB];
            auto it = danhSachPhong.find(ma);
            if (it != danhSachPhong.end())
                data[idxTenPB] = it->second;
            else
                data[idxTenPB] = "";
        }
    };

    // Nếu đang ở trường mã phòng ban, xác định selection initial
    if (idxMaPB != -1 && !data[idxMaPB].empty()) {
        for (int i = 0; i < dsMaPhong.size(); ++i)
            if (data[idxMaPB] == dsMaPhong[i]) comboBoxSelection = i;
    }

    drawAll();

    while (true) {
        if (cur == idxMaPB) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int key = _getch();
                if (key == 72) { // Up
                    comboBoxSelection = (comboBoxSelection - 1 + dsMaPhong.size()) % dsMaPhong.size();
                    drawField(cur, true);
                } else if (key == 80) { // Down
                    comboBoxSelection = (comboBoxSelection + 1) % dsMaPhong.size();
                    drawField(cur, true);
                }
            } else if (ch == 13) { // Enter chọn mã phòng ban
                data[idxMaPB] = dsMaPhong[comboBoxSelection];
                autoFillTenPhongBan();
                drawField(idxMaPB, true);
                drawField(idxTenPB, false);
                cur = nextField(cur, 1);
                drawAll();
            } else if (ch == 27) {
                clearScreen();
                return "";
            }
        } else {
            int inputX = inputPositions[cur] + data[cur].length();
            int inputY = startY + cur * fieldHeight + 1;
            gotoxy(inputX, inputY);
            setColor(14);

            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int key = _getch();
                int prevSelected = cur;
                if (key == 72) cur = nextField(cur, -1);
                if (key == 80) cur = nextField(cur, 1);
                if (prevSelected != cur) {
                    for (int i : {prevSelected, cur}) drawField(i, i == cur);
                }
            } else if (ch == 13) {
                if (cur == label.size() - 1) {
                    clearScreen();
                    return "OK";
                } else {
                    drawField(cur, false);
                    cur = nextField(cur, 1);
                    drawField(cur, true);
                }
            } else if (ch == 27) {
                clearScreen();
                return "";
            } else if (ch == 8) {
                if (!data[cur].empty()) {
                    data[cur].pop_back();
                    gotoxy(inputPositions[cur], startY + cur * fieldHeight + 1);
                    cout << string(data[cur].length() + 1, ' ');
                    gotoxy(inputPositions[cur], startY + cur * fieldHeight + 1);
                    cout << data[cur];
                    gotoxy(inputPositions[cur] + data[cur].length(), startY + cur * fieldHeight + 1);
                }
            } else if (ch >= 32 && ch <= 126) {
                if (data[cur].length() < 50) {
                    if (label[cur] == "Luong (nghin VND)" && !isdigit(ch) && ch != '.') continue;
                    if (label[cur] == "Ma nhan vien (8 chu so)" && !isalnum(ch)) continue;
                    data[cur].push_back((char)ch);
                    cout << (char)ch;
                }
            }
        }
    }
}

void themMoiNhanVien(vector<NhanVien>& ds) {
    vector<string> label = {
        "Ma phong ban", "Ten phong ban", "Ma nhan vien (8 chu so)", "Ho ten", "Chuc vu",
        "Ngay sinh (dd mm yyyy)", "Luong (nghin VND)"
    };
    vector<string> data(7);
    vector<bool> invalidFields;

    while (true) {
        string result = nhapVaChinhSuaTrenConsole(label, data, invalidFields, ds);
        if (result.empty()) return;

        NhanVien nv;
        nv.maPhongBan = inHoaChuoi(trim(data[0]));
        nv.tenPhongBan = chuanHoaTen(trim(data[1]));
        nv.maNhanVien = trim(data[2]);
        nv.hoTen = chuanHoaTen(trim(data[3]));
        nv.chucVu = chuanHoaTen(trim(data[4]));

        invalidFields.clear();
        invalidFields.resize(label.size(), false);
        bool hasError = false;

        for (int i = 0; i < data.size(); i++) {
            if (trim(data[i]).empty()) {
                invalidFields[i] = true;
                hasError = true;
            }
        }

        if (isDuplicateID(ds, nv.maNhanVien)) {
            invalidFields[2] = true;
            hasError = true;
        }

        stringstream ss(data[5]);
        if (!(ss >> nv.ngaySinh.day >> nv.ngaySinh.month >> nv.ngaySinh.year) || !ss.eof()) {
            invalidFields[5] = true;
            hasError = true;
        } else if (!ngayHopLe(nv.ngaySinh) || !tuoiHopLe(nv.ngaySinh)) {
            invalidFields[5] = true;
            hasError = true;
        }

        try {
            nv.luong = stod(trim(data[6]));
            if (!luongHopLe(nv.luong)) {
                invalidFields[6] = true;
                hasError = true;
            }
        } catch (const std::invalid_argument&) {
            invalidFields[6] = true;
            hasError = true;
        } catch (const std::out_of_range&) {
            invalidFields[6] = true;
            hasError = true;
        }

        if (!maNhanVienHopLe(nv)) {
            invalidFields[2] = true;
            hasError = true;
        }

        if (!hasError) {
            ds.push_back(nv);
            if (!ghiFile(ds, "nhanvien.dat")) {
                cout << "Lỗi khi ghi file!\n";
                system("pause");
                return;
            }
            clearScreen();
            cout << "Da them thanh cong!\n";
            system("pause");
            return;
        }
    }
}
void printWithHighlight(const string& content, const string& keyword, int highlightColor = 14, int normalColor = 7) {
    if (keyword.empty()) {
        setColor(normalColor);
        cout << content;
        return;
    }
    size_t pos = 0, prev = 0;
    string contentLower = content;
    string keywordLower = keyword;
    transform(contentLower.begin(), contentLower.end(), contentLower.begin(), ::tolower);
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

    while ((pos = contentLower.find(keywordLower, prev)) != string::npos) {
        setColor(normalColor);
        cout << content.substr(prev, pos - prev);
        setColor(highlightColor);
        cout << content.substr(pos, keyword.size());
        prev = pos + keyword.size();
    }
    setColor(normalColor);
    cout << content.substr(prev);
}

void inDanhSach(const vector<NhanVien>& ds, const string& note, const string& keyword) {
    const int perPage = 10;
    int totalPage = (ds.size() + perPage - 1) / perPage;
    int currentPage = 0;

    const int widthMa = 12;
    const int widthTen = 25;
    const int widthPhong = 15;
    const int widthTenPhong = 25;
    const int widthChucVu = 18;
    const int widthNgaySinh = 14;
    const int widthLuong = 20;

    auto printSeparator = [&]() {
        cout << "+";
        cout << string(widthMa, '-') << "+";
        cout << string(widthTen, '-') << "+";
        cout << string(widthPhong, '-') << "+";
        cout << string(widthTenPhong, '-') << "+";
        cout << string(widthChucVu, '-') << "+";
        cout << string(widthNgaySinh, '-') << "+";
        cout << string(widthLuong, '-') << "+\n";
    };

    auto formatSalary = [](double salary) {
        double salaryInThousands = salary * 1000;
        stringstream ss;
        ss.imbue(locale(""));
        ss << fixed << setprecision(0) << salaryInThousands << " VND";
        return ss.str();
    };

    while (true) {
        clearScreen();

        if (ds.empty()) {
            cout << "Danh sach nhan vien trong!\n";
            system("pause");
            return;
        }

        // Tiêu đề bảng căn giữa
        string title = "=== DANH SACH NHAN VIEN (Trang " + to_string(currentPage + 1) + "/" + to_string(max(totalPage, 1)) + ") ===";
        int tableWidth = widthMa + widthTen + widthPhong + widthTenPhong + widthChucVu + widthNgaySinh + widthLuong + 8;
        int titleX = max(0, (tableWidth - (int)title.length()) / 2);
        gotoxy(titleX, 1);
        setColor(14);
        cout << title << endl;
        setColor(7);

        if (!note.empty()) {
            setColor(11);
            cout << note << endl;
            setColor(7);
        }

        printSeparator();

        // Header nổi bật
        setColor(11);
        cout << "|"
             << setw(widthMa)      << left << "Ma NV"      << "|"
             << setw(widthTen)     << left << "Ho Ten"     << "|"
             << setw(widthPhong)   << left << "Ma Phong"   << "|"
             << setw(widthTenPhong)<< left << "Ten Phong"  << "|"
             << setw(widthChucVu)  << left << "Chuc Vu"    << "|"
             << setw(widthNgaySinh)<< left << "Ngay Sinh"  << "|"
             << setw(widthLuong)   << right << "Luong"     << "|\n";
        setColor(7);

        printSeparator();

        int start = currentPage * perPage;
        int end = min((int)ds.size(), start + perPage);

        for (int i = start; i < end; ++i) {
            const auto& nv = ds[i];
            stringstream ss;
            ss << setfill('0') << setw(2) << nv.ngaySinh.day << "/"
               << setfill('0') << setw(2) << nv.ngaySinh.month << "/"
               << setw(4) << nv.ngaySinh.year;
            string ngaySinh = ss.str();
            string luong = formatSalary(nv.luong);

            cout << "|";
            printWithHighlight(nv.maNhanVien, keyword, 12, 7);
            cout << string(widthMa - nv.maNhanVien.length(), ' ') << "|";
            printWithHighlight(nv.hoTen, keyword, 12, 7);
            cout << string(widthTen - nv.hoTen.length(), ' ') << "|";
            printWithHighlight(nv.maPhongBan, keyword, 12, 7);
            cout << string(widthPhong - nv.maPhongBan.length(), ' ') << "|";
            printWithHighlight(nv.tenPhongBan, keyword, 12, 7);
            cout << string(widthTenPhong - nv.tenPhongBan.length(), ' ') << "|";
            printWithHighlight(nv.chucVu, keyword, 12, 7);
            cout << string(widthChucVu - nv.chucVu.length(), ' ') << "|";
            printWithHighlight(ngaySinh, keyword, 12, 7);
            cout << string(widthNgaySinh - ngaySinh.length(), ' ') << "|";
            printWithHighlight(luong, keyword, 12, 7);
            cout << string(widthLuong - luong.length(), ' ') << "|\n";
        }

        printSeparator();
        cout << "[A] Trang truoc  |  [D] Trang sau  |  [ESC] Thoat\n";

        char ch = _getch();
        if (ch == 27) break;
        else if ((ch == 'a' || ch == 'A') && totalPage > 1) {
            currentPage = (currentPage - 1 + totalPage) % totalPage;
        } else if ((ch == 'd' || ch == 'D') && totalPage > 1) {
            currentPage = (currentPage + 1) % totalPage;
        }
    }
}

bool ghiFile(const vector<NhanVien>& ds, const string& filename) {
    ofstream f(filename, ios::binary);
    if (!f) return false;
    int size = ds.size();
    f.write((char*)&size, sizeof(int));
    for (const auto& nv : ds) {
        vector<string> fields = {
            nv.maPhongBan, nv.tenPhongBan, nv.maNhanVien,
            nv.hoTen, nv.chucVu
        };
        for (auto& s : fields) {
            int len = s.size();
            f.write((char*)&len, sizeof(int));
            f.write(s.c_str(), len);
        }
        f.write((char*)&nv.ngaySinh, sizeof(Date));
        f.write((char*)&nv.luong, sizeof(double));
    }
    return f.good();
}

bool docFile(vector<NhanVien>& ds, const string& filename) {
    ds.clear();
    ifstream f(filename, ios::binary);
    if (!f) return false;
    int size;
    f.read((char*)&size, sizeof(int));
    for (int i = 0; i < size && f; ++i) {
        NhanVien nv;
        for (int fi = 0; fi < 5; ++fi) {
            int len;
            f.read((char*)&len, sizeof(int));
            string str(len, '\0');
            f.read(&str[0], len);
            switch (fi) {
                case 0: nv.maPhongBan = str; break;
                case 1: nv.tenPhongBan = str; break;
                case 2: nv.maNhanVien = str; break;
                case 3: nv.hoTen = str; break;
                case 4: nv.chucVu = str; break;
            }
        }
        f.read((char*)&nv.ngaySinh, sizeof(Date));
        f.read((char*)&nv.luong, sizeof(double));
        ds.push_back(nv);
    }
    return true;
}