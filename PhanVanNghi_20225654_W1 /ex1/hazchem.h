#ifndef HAZCHEM_H
#define HAZCHEM_H

/**
 * @struct Advice: Ket qua tra bang cho mot ma HAZCHEM hop le.
 *
 * reactivity co gia tri NULL neu khong hien thi dong Reactivity.
 */
typedef struct {
    const char *material;
    const char *reactivity;
    const char *protection;
    const char *containment;
    int hasEvacuation;
} Advice;

/**
 * @function checkCode: Kiem tra chuoi nhap co dung dinh dang ma HAZCHEM khong.
 *
 * @param code: Chuoi ma da duoc cat khoang trang.
 * @param firstChar: Con tro nhan ky tu thu nhat ('1'..'4') neu hop le.
 * @param letterChar: Con tro nhan ky tu thu hai (da doi thanh in hoa) neu hop le.
 * @param hasEvacuation: Con tro nhan 1 neu ma co ky tu E o cuoi, 0 neu khong.
 *
 * @return: 1 neu ma hop le.
 *          0 neu sai do dai, sai ky tu dau, sai ky tu hai hoac sai ky tu ba.
 */
int checkCode(const char *code, char *firstChar, char *letterChar,
              int *hasEvacuation);

/**
 * @function needsColourQuestion: Kiem tra ky tu thu hai co the co 2 mau khong.
 *
 * @param letterChar: Ky tu thu hai, da la chu in hoa.
 *
 * @return: 1 neu letterChar la mot trong S, T, Y, Z.
 *          0 neu nguoc lai.
 */
int needsColourQuestion(char letterChar);

/**
 * @function buildAdvice: Tra bang va dien day du thong tin advice.
 *
 * @param firstChar: Ky tu thu nhat cua ma ('1'..'4').
 * @param letterChar: Ky tu thu hai cua ma, da la chu in hoa.
 * @param reversed: 1 neu ky tu do duoc in dao mau, 0 neu khong.
 * @param hasEvacuation: 1 neu ma co ky tu E o cuoi, 0 neu khong.
 * @param advice: Con tro toi cau truc se duoc dien thong tin.
 *
 * @return: nothing.
 */
void buildAdvice(char firstChar, char letterChar, int reversed,
                 int hasEvacuation, Advice *advice);

/**
 * @function printAdvice: In huong dan hanh dong khan cap ra man hinh.
 *
 * @param advice: Con tro toi thong tin advice can in.
 *
 * @return: nothing.
 */
void printAdvice(const Advice *advice);

#endif
