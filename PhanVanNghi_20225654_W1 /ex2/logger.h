#ifndef LOGGER_H
#define LOGGER_H

/**
 * @function writeLog: Ghi mot dong nhat ky hoat dong vao file log.
 *
 * Dinh dang dong ghi ra: [dd/mm/yyyy hh:mm:ss] $ function $ value $ ket qua
 * Moi ky tu '$' trong value duoc thay bang "\$" truoc khi ghi.
 *
 * @param path: Duong dan file log (mo o che do append).
 * @param function: So thu tu chuc nang duoc chon ("1".."4").
 * @param value: Gia tri nguoi dung nhap, hoac chuoi rong.
 * @param ok: 1 de ghi +OK, 0 de ghi -ERR.
 *
 * @return: nothing. Neu khong mo duoc file log, in canh bao ra man hinh.
 */
void writeLog(const char *path, const char *function, const char *value, int ok);

#endif
