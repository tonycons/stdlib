/*
   Copyright 2025 Anthony A. Constantinescu.

   Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
   in compliance with the License. You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed under the License
   is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the License for the specific language governing permissions and limitations under
   the License.

   File: commons/system/syscall_linux_x64_numbers.inl
   Description: Defines the numbers of linux syscalls for x64
*/

#ifdef __inline_system_header__

UNSAFE_BEGIN;


///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n) noexcept
{
    u64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1) noexcept
{
    u64 ret;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1, auto p2) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);

    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory");
    return result;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1, auto p2, auto p3) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);

    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");

    return result;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);

    register u64 r10 asm("r10") = a4;
    asm volatile("syscall" : "=a"(result) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10) : "rcx", "r11", "memory");

    return result;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4, auto p5) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);
    auto a5 = *reinterpret_cast<u64 const*>(&p5);

    register u64 r10 asm("r10") = a4;
    register u64 r8 asm("r8") = a5;

    asm volatile("syscall"
                 : "=a"(result)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
                 : "rcx", "r11", "memory");

    return result;
}

///
///
///
[[maybe_unused]] FORCEINLINE static u64 Syscall(int n, auto p1, auto p2, auto p3, auto p4, auto p5, auto p6) noexcept
{
    u64 result;

    auto a1 = *reinterpret_cast<u64 const*>(&p1);
    auto a2 = *reinterpret_cast<u64 const*>(&p2);
    auto a3 = *reinterpret_cast<u64 const*>(&p3);
    auto a4 = *reinterpret_cast<u64 const*>(&p4);
    auto a5 = *reinterpret_cast<u64 const*>(&p5);
    auto a6 = *reinterpret_cast<u64 const*>(&p6);

    register u64 r10 asm("r10") = a4;
    register u64 r8 asm("r8") = a5;
    register u64 r9 asm("r9") = a6;

    asm volatile("syscall"
                 : "=a"(result)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
                 : "rcx", "r11", "memory");

    return result;
}

UNSAFE_END;

// source: https://gpages.juszkiewicz.com.pl/syscalls-table/syscalls.html
// NOLINTBEGIN(readability-identifier-naming)

enum {
    waitpid = -1,
    vm86old = -1,
    vm86 = -1,
    utrap_install = -1,
    umount = -1,
    timerfd = -1,
    sysmips = -1,
    syscall = -1,
    sys_debug_setcontext = -1,
    switch_endian = -1,
    swapcontext = -1,
    subpage_prot = -1,
    stime = -1,
    ssetmask = -1,
    spu_run = -1,
    spu_create = -1,
    socketcall = -1,
    signal = -1,
    sgetmask = -1,
    setpgrp = -1,
    sethae = -1,
    sched_set_affinity = -1,
    sched_get_affinity = -1,
    s390_sthyi = -1,
    s390_runtime_instr = -1,
    s390_pci_mmio_write = -1,
    s390_pci_mmio_read = -1,
    s390_guarded_storage = -1,
    rtas = -1,
    riscv_hwprobe = -1,
    riscv_flush_icache = -1,
    readdir = -1,
    perfctr = -1,
    osf_wait4 = -1,
    osf_utsname = -1,
    osf_utimes = -1,
    osf_usleep_thread = -1,
    osf_sysinfo = -1,
    osf_syscall = -1,
    osf_swapon = -1,
    osf_statfs64 = -1,
    osf_statfs = -1,
    osf_stat = -1,
    osf_sigstack = -1,
    osf_sigprocmask = -1,
    osf_shmat = -1,
    osf_settimeofday = -1,
    osf_setsysinfo = -1,
    osf_setitimer = -1,
    osf_set_program_attributes = -1,
    osf_select = -1,
    osf_proplist_syscall = -1,
    osf_mount = -1,
    osf_lstat = -1,
    osf_gettimeofday = -1,
    osf_getsysinfo = -1,
    osf_getrusage = -1,
    osf_getitimer = -1,
    osf_getdomainname = -1,
    osf_getdirentries = -1,
    osf_fstatfs64 = -1,
    osf_fstatfs = -1,
    osf_fstat = -1,
    or1k_atomic = -1,
    olduname = -1,
    oldumount = -1,
    oldstat = -1,
    oldolduname = -1,
    oldlstat = -1,
    oldfstat = -1,
    old_adjtimex = -1,
    multiplexer = -1,
    memory_ordering = -1,
    kern_features = -1,
    ipc = -1,
    getxuid = -1,
    getxpid = -1,
    getxgid = -1,
    getpagesize = -1,
    gethostname = -1,
    getdtablesize = -1,
    getdomainname = -1,
    fadvise64_64 = -1,
    execv = -1,
    exec_with_loader = -1,
    dipc = -1,
    cacheflush = -1,
    cachectl = -1,
    atomic_cmpxchg_32 = -1,
    atomic_barrier = -1,
    arc_usr_cmpxchg = -1,
    arc_settls = -1,
    arc_gettls = -1,
    nice = -1,
    sigaction = -1,
    sigsuspend = -1,
    sigpending = -1,
    sigreturn = -1,
    sigprocmask = -1,
    _llseek = -1,
    _newselect = -1,
    ugetrlimit = -1,
    mmap2 = -1,
    truncate64 = -1,
    ftruncate64 = -1,
    stat64 = -1,
    lstat64 = -1,
    fstat64 = -1,
    lchown32 = -1,
    getuid32 = -1,
    getgid32 = -1,
    geteuid32 = -1,
    getegid32 = -1,
    setreuid32 = -1,
    setregid32 = -1,
    getgroups32 = -1,
    setgroups32 = -1,
    fchown32 = -1,
    setresuid32 = -1,
    getresuid32 = -1,
    setresgid32 = -1,
    getresgid32 = -1,
    chown32 = -1,
    setuid32 = -1,
    setgid32 = -1,
    setfsuid32 = -1,
    setfsgid32 = -1,
    fcntl64 = -1,
    sendfile64 = -1,
    statfs64 = -1,
    fstatfs64 = -1,
    arm_fadvise64_64 = -1,
    pciconfig_iobase = -1,
    pciconfig_read = -1,
    pciconfig_write = -1,
    send = -1,
    recv = -1,
    fstatat64 = -1,
    sync_file_range2 = -1,
    clock_gettime64 = -1,
    clock_settime64 = -1,
    clock_adjtime64 = -1,
    clock_getres_time64 = -1,
    clock_nanosleep_time64 = -1,
    timer_gettime64 = -1,
    timer_settime64 = -1,
    timerfd_gettime64 = -1,
    timerfd_settime64 = -1,
    utimensat_time64 = -1,
    pselect6_time64 = -1,
    ppoll_time64 = -1,
    io_pgetevents_time64 = -1,
    recvmmsg_time64 = -1,
    mq_timedsend_time64 = -1,
    mq_timedreceive_time64 = -1,
    semtimedop_time64 = -1,
    rt_sigtimedwait_time64 = -1,
    futex_time64 = -1,
    sched_rr_get_interval_time64 = -1,
    read = 0,
    write = 1,
    open = 2,
    close = 3,
    stat = 4,
    fstat = 5,
    lstat = 6,
    poll = 7,
    lseek = 8,
    mmap = 9,
    mprotect = 10,
    munmap = 11,
    brk = 12,
    rt_sigaction = 13,
    rt_sigprocmask = 14,
    rt_sigreturn = 15,
    ioctl = 16,
    pread64 = 17,
    pwrite64 = 18,
    readv = 19,
    writev = 20,
    access = 21,
    pipe = 22,
    select = 23,
    sched_yield = 24,
    mremap = 25,
    msync = 26,
    mincore = 27,
    madvise = 28,
    shmget = 29,
    shmat = 30,
    shmctl = 31,
    dup = 32,
    dup2 = 33,
    pause = 34,
    nanosleep = 35,
    getitimer = 36,
    alarm = 37,
    setitimer = 38,
    getpid = 39,
    sendfile = 40,
    socket = 41,
    connect = 42,
    accept = 43,
    sendto = 44,
    recvfrom = 45,
    sendmsg = 46,
    recvmsg = 47,
    shutdown = 48,
    bind = 49,
    listen = 50,
    getsockname = 51,
    getpeername = 52,
    socketpair = 53,
    setsockopt = 54,
    getsockopt = 55,
    clone = 56,
    fork = 57,
    vfork = 58,
    execve = 59,
    exit = 60,
    wait4 = 61,
    kill = 62,
    uname = 63,
    semget = 64,
    semop = 65,
    semctl = 66,
    shmdt = 67,
    msgget = 68,
    msgsnd = 69,
    msgrcv = 70,
    msgctl = 71,
    fcntl = 72,
    flock = 73,
    fsync = 74,
    fdatasync = 75,
    truncate = 76,
    ftruncate = 77,
    getdents = 78,
    getcwd = 79,
    chdir = 80,
    fchdir = 81,
    rename = 82,
    mkdir = 83,
    rmdir = 84,
    creat = 85,
    link = 86,
    unlink = 87,
    symlink = 88,
    readlink = 89,
    chmod = 90,
    fchmod = 91,
    chown = 92,
    fchown = 93,
    lchown = 94,
    umask = 95,
    gettimeofday = 96,
    getrlimit = 97,
    getrusage = 98,
    sysinfo = 99,
    times = 100,
    ptrace = 101,
    getuid = 102,
    syslog = 103,
    getgid = 104,
    setuid = 105,
    setgid = 106,
    geteuid = 107,
    getegid = 108,
    setpgid = 109,
    getppid = 110,
    getpgrp = 111,
    setsid = 112,
    setreuid = 113,
    setregid = 114,
    getgroups = 115,
    setgroups = 116,
    setresuid = 117,
    getresuid = 118,
    setresgid = 119,
    getresgid = 120,
    getpgid = 121,
    setfsuid = 122,
    setfsgid = 123,
    getsid = 124,
    capget = 125,
    capset = 126,
    rt_sigpending = 127,
    rt_sigtimedwait = 128,
    rt_sigqueueinfo = 129,
    rt_sigsuspend = 130,
    sigaltstack = 131,
    utime = 132,
    mknod = 133,
    personality = 135,
    ustat = 136,
    statfs = 137,
    fstatfs = 138,
    sysfs = 139,
    getpriority = 140,
    setpriority = 141,
    sched_setparam = 142,
    sched_getparam = 143,
    sched_setscheduler = 144,
    sched_getscheduler = 145,
    sched_get_priority_max = 146,
    sched_get_priority_min = 147,
    sched_rr_get_interval = 148,
    mlock = 149,
    munlock = 150,
    mlockall = 151,
    munlockall = 152,
    vhangup = 153,
    modify_ldt = 154,
    pivot_root = 155,
    prctl = 157,
    arch_prctl = 158,
    adjtimex = 159,
    setrlimit = 160,
    chroot = 161,
    sync = 162,
    acct = 163,
    settimeofday = 164,
    mount = 165,
    umount2 = 166,
    swapon = 167,
    swapoff = 168,
    reboot = 169,
    sethostname = 170,
    setdomainname = 171,
    iopl = 172,
    ioperm = 173,
    init_module = 175,
    delete_module = 176,
    quotactl = 179,
    gettid = 186,
    readahead = 187,
    setxattr = 188,
    lsetxattr = 189,
    fsetxattr = 190,
    getxattr = 191,
    lgetxattr = 192,
    fgetxattr = 193,
    listxattr = 194,
    llistxattr = 195,
    flistxattr = 196,
    removexattr = 197,
    lremovexattr = 198,
    fremovexattr = 199,
    tkill = 200,
    time = 201,
    futex = 202,
    sched_setaffinity = 203,
    sched_getaffinity = 204,
    set_thread_area = 205,
    io_setup = 206,
    io_destroy = 207,
    io_getevents = 208,
    io_submit = 209,
    io_cancel = 210,
    get_thread_area = 211,
    lookup_dcookie = 212,
    epoll_create = 213,
    epoll_ctl_old = 214,
    epoll_wait_old = 215,
    remap_file_pages = 216,
    getdents64 = 217,
    set_tid_address = 218,
    restart_syscall = 219,
    semtimedop = 220,
    fadvise64 = 221,
    timer_create = 222,
    timer_settime = 223,
    timer_gettime = 224,
    timer_getoverrun = 225,
    timer_delete = 226,
    clock_settime = 227,
    clock_gettime = 228,
    clock_getres = 229,
    clock_nanosleep = 230,
    exit_group = 231,
    epoll_wait = 232,
    epoll_ctl = 233,
    tgkill = 234,
    utimes = 235,
    mbind = 237,
    set_mempolicy = 238,
    get_mempolicy = 239,
    mq_open = 240,
    mq_unlink = 241,
    mq_timedsend = 242,
    mq_timedreceive = 243,
    mq_notify = 244,
    mq_getsetattr = 245,
    kexec_load = 246,
    waitid = 247,
    add_key = 248,
    request_key = 249,
    keyctl = 250,
    ioprio_set = 251,
    ioprio_get = 252,
    inotify_init = 253,
    inotify_add_watch = 254,
    inotify_rm_watch = 255,
    migrate_pages = 256,
    openat = 257,
    mkdirat = 258,
    mknodat = 259,
    fchownat = 260,
    futimesat = 261,
    newfstatat = 262,
    unlinkat = 263,
    renameat = 264,
    linkat = 265,
    symlinkat = 266,
    readlinkat = 267,
    fchmodat = 268,
    faccessat = 269,
    pselect6 = 270,
    ppoll = 271,
    unshare = 272,
    set_robust_list = 273,
    get_robust_list = 274,
    splice = 275,
    tee = 276,
    sync_file_range = 277,
    vmsplice = 278,
    move_pages = 279,
    utimensat = 280,
    epoll_pwait = 281,
    signalfd = 282,
    timerfd_create = 283,
    eventfd = 284,
    fallocate = 285,
    timerfd_settime = 286,
    timerfd_gettime = 287,
    accept4 = 288,
    signalfd4 = 289,
    eventfd2 = 290,
    epoll_create1 = 291,
    dup3 = 292,
    pipe2 = 293,
    inotify_init1 = 294,
    preadv = 295,
    pwritev = 296,
    rt_tgsigqueueinfo = 297,
    perf_event_open = 298,
    recvmmsg = 299,
    fanotify_init = 300,
    fanotify_mark = 301,
    prlimit64 = 302,
    name_to_handle_at = 303,
    open_by_handle_at = 304,
    clock_adjtime = 305,
    syncfs = 306,
    sendmmsg = 307,
    setns = 308,
    getcpu = 309,
    process_vm_readv = 310,
    process_vm_writev = 311,
    kcmp = 312,
    finit_module = 313,
    sched_setattr = 314,
    sched_getattr = 315,
    renameat2 = 316,
    seccomp = 317,
    getrandom = 318,
    memfd_create = 319,
    kexec_file_load = 320,
    bpf = 321,
    execveat = 322,
    userfaultfd = 323,
    membarrier = 324,
    mlock2 = 325,
    copy_file_range = 326,
    preadv2 = 327,
    pwritev2 = 328,
    pkey_mprotect = 329,
    pkey_alloc = 330,
    pkey_free = 331,
    statx = 332,
    io_pgetevents = 333,
    rseq = 334,
    pidfd_send_signal = 424,
    io_uring_setup = 425,
    io_uring_enter = 426,
    io_uring_register = 427,
    open_tree = 428,
    move_mount = 429,
    fsopen = 430,
    fsconfig = 431,
    fsmount = 432,
    fspick = 433,
    pidfd_open = 434,
    clone3 = 435,
    close_range = 436,
    openat2 = 437,
    pidfd_getfd = 438,
    faccessat2 = 439,
    process_madvise = 440,
    epoll_pwait2 = 441,
    mount_setattr = 442,
    quotactl_fd = 443,
    landlock_create_ruleset = 444,
    landlock_add_rule = 445,
    landlock_restrict_self = 446,
    memfd_secret = 447,
    process_mrelease = 448,
    futex_waitv = 449,
    set_mempolicy_home_node = 450,
    cachestat = 451,
    fchmodat2 = 452,
    map_shadow_stack = 453,
    futex_wake = 454,
    futex_wait = 455,
    futex_requeue = 456,
    statmount = 457,
    listmount = 458,
    lsm_get_self_attr = 459,
    lsm_set_self_attr = 460,
    lsm_list_modules = 461
};

// NOLINTEND(readability-identifier-naming)
#endif
