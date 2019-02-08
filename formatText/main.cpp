//
//  main.cpp
//  formatText
//
//  Created by mark on 2019/2/7.
//  Copyright © 2019 mark. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
using namespace std;

std::string UnicodeToUTF8(const std::wstring &wstr)
{
    std::string ret;
    try
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
        ret = wcv.to_bytes(wstr);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return ret;
}

std::wstring UTF8ToUnicode(const std::string &str)
{
    std::wstring ret;
    try
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
        ret = wcv.from_bytes(str);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return ret;
}

bool isChinese(wchar_t c)
{
    if (c > 0x4e00 && c < 0x9fff)
    {
        return true;
    }
    return false;
}
int main(int argc, const char *argv[])
{
    const std::locale utf8(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wcout.imbue(utf8);
    fstream fin;
    fstream fout;
    string inputPath;
    string outputPath;
    cout << "请输入要格式化的文本文件路径：";
    getline(cin, inputPath);
    cout << "请输入输出路径：";
    getline(cin, outputPath);
    fin.open(inputPath, ios::in);
    fout.open(outputPath, ios::out);
    string s;
    wstring ws;
    if (fin.is_open() && fout.is_open())
    {
        while (getline(fin, s))
        {
            ws = UTF8ToUnicode(s);
            cout << "开始逐行按照文案要求转换..." << endl;
            // 如果不是空行
            int len = ws.length();
            if (len > 0)
            {
                for (int i = 0; i < len - 1; i++)
                {
                    if (iswalnum(ws[i]) && isChinese(ws[i + 1]))
                    {
                        // 英文和数字挨着一个中文字符
                        wchar_t tmp[2] = {ws[i], 0};
                        wstring tmpStr = tmp;
                        fout << UnicodeToUTF8(tmpStr) << " ";
                    }
                    else if (iswalnum(ws[i + 1]) && isChinese(ws[i]))
                    {
                        // 一个中文字符挨着英文和数字
                        wchar_t tmp[2] = {ws[i], 0};
                        wstring tmpStr = tmp;
                        fout << UnicodeToUTF8(tmpStr) << " ";
                    }
                    else
                    {
                        // 进行复制
                        wchar_t tmp[2] = {ws[i], 0};
                        wstring tmpStr = tmp;
                        fout << UnicodeToUTF8(tmpStr);
                    }
                }
                // 进行复制
                wchar_t tmp[2] = {ws[len - 1], 0};
                wstring tmpStr = tmp;
                fout << UnicodeToUTF8(tmpStr);
            }
            fout << endl;
        }
    }

    fin.close();
    fout.close();
    cout << "转换完成" << endl;
    return 0;
}
