#include "stdafx.h"
#include "myfile.h"
#include "mytxt.h"

txt *file_error;
txt *file_error_name;

file *open(LPCSTR name) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(name, &fs, OF_READWRITE | OF_SHARE_COMPAT);
    if (tf->h == HFILE_ERROR) {
        file_error_name = txtnew();
        txtset(file_error_name, name);
        file_error = txtnew();
        txtset(file_error, "File ");
        txtadd(file_error, file_error_name);
        txtadd(file_error, " not found");
        MessageBox(nullptr, file_error->d, "Ultima 6 Online", MB_OK);
    }
    return tf;
}

file *open2(LPCSTR name, unsigned long flags) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(name, &fs, flags);
    return tf;
}

file *open2(txt *t, unsigned long flags) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(t->d, &fs, flags);
    return tf;
}

file *open(txt *t) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(t->d, &fs, OF_READWRITE | OF_SHARE_COMPAT);
    if (tf->h == HFILE_ERROR) {
        txtset(file_error_name, t);
        file_error = txtnew();
        txtset(file_error, "File ");
        txtadd(file_error, file_error_name);
        txtadd(file_error, " not found");
        MessageBox(nullptr, file_error->d, "Ultima 6 Online", MB_OK);
    }
    return tf;
}

void get(file *filePtr, void *dest, long bytesToRead) {
    if (filePtr->h != HFILE_ERROR)
        _hread(filePtr->h, dest, bytesToRead);
}

void loadFromFile(LPCSTR name, void *dest, long bytesToRead) {
    static file *f;
    f = open(name);

    if (bytesToRead == MAXLONG) {
        bytesToRead = fileLen(f);
    }

    if (f->h != HFILE_ERROR) {
        _hread(f->h, dest, bytesToRead);
    }
    close(f);
}

file *loadFromFileAndReturn(LPCSTR name, void *dest, long bytesToRead) {
    static file *f;
    f = open(name);

    if (bytesToRead == MAXLONG) {
        bytesToRead = fileLen(f);
    }

    if (f->h != HFILE_ERROR) {
        _hread(f->h, dest, bytesToRead);
    }

    return f;
}

template<typename T>
void loadFromFileMalloc(LPCSTR name, T *dest, long bytesToRead) {
    static file *f;
    f = open(name);
    if (f->h != HFILE_ERROR) {
        dest=(T*)malloc(fileLen(f));
        _hread(f->h, dest, bytesToRead);
    }
    close(f);
}

void put(file *filePtr, void *src, long bytesToWrite) {
    if (filePtr->h != HFILE_ERROR)
        _hwrite(filePtr->h, (LPCSTR) src, bytesToWrite);
}

void seek(file *filePtr, long bytesToSeek) {
    if (filePtr->h != HFILE_ERROR)
        _llseek(filePtr->h, bytesToSeek, FILE_BEGIN);
}

void close(file *filePtr) {
    if (filePtr->h != HFILE_ERROR)
        _lclose(filePtr->h);
    free((void *) filePtr);
}

long seek(file *filePtr) {
    if (filePtr->h == HFILE_ERROR)
        return 0;
    return _llseek(filePtr->h, 0, FILE_CURRENT);
}

long fileLen(file *filePtr) {
    if (filePtr->h == HFILE_ERROR) return 0;
    long currentPos = _llseek(filePtr->h, 0, FILE_CURRENT);
    long endPos = _llseek(filePtr->h, 0, FILE_END);
    _llseek(filePtr->h, currentPos, FILE_BEGIN);
    return endPos;
}

long lof(file *filePtr) {
    if (filePtr->h == HFILE_ERROR) return 0;
    static long i, i2;
    i = _llseek(filePtr->h, 0, FILE_CURRENT);
    i2 = _llseek(filePtr->h, 0, FILE_END);
    _llseek(filePtr->h, i, FILE_BEGIN);
    return i2;
}

void *loadfile(LPCSTR name) {
    long loadfile_FILESIZE;
    static file *f;
    static void *v;
    f = open(name);
    if (f->h != HFILE_ERROR)
        return nullptr;
    loadfile_FILESIZE = fileLen(f);
    v = malloc(loadfile_FILESIZE);
    get(f, v, loadfile_FILESIZE);
    close(f);
    return v;
}

void waitforfile(LPCSTR name) {
    static OFSTRUCT fs;
    static HFILE hfile;
    bool retry = true;

    /// Cocoa - This needs to be evaluated for how long some files need to load.
    ///         We can get stuck here forever if the file is not available.
    ///         We should add a sleep and a retry limit. \n\n
    ///
    ///
    ///         Additionally, I am unsure what the purpose of this function is.
    ///         It seems to be opening a file and then immediately closing it.
    ///         If we are just checking for the existence of a file, why are
    ///         we continually retrying to open it and not returning an error?
    while (retry) {
        hfile = OpenFile(name, &fs, OF_SHARE_EXCLUSIVE | OF_READWRITE);
        if (hfile == HFILE_ERROR)
            retry = true;
        else
            retry = false;
    }

    _lclose(hfile);
}

void deletefile(LPCSTR name) {
    static OFSTRUCT fs;
    OpenFile(name, &fs, OF_DELETE);
}