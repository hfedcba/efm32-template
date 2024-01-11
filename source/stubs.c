//Stubs to avoid linker warnings with ARM GCC >= 11.3

void _close() {}
void _getpid_r() {}
void _kill_r() {}
void _lseek() {}
void _read() {}
void _isatty() {}
void _fstat() {}
#ifndef SEGGER_RTT
void _write() {}
#endif