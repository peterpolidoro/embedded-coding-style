# 43 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qep_hsm")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif





char const QP_versionStr[8] = QP_VERSION_STR;






QEvt const QEvt_reserved_[4] = {
    QEVT_INITIALIZER(Q_EMPTY_SIG),
    QEVT_INITIALIZER(Q_ENTRY_SIG),
    QEVT_INITIALIZER(Q_EXIT_SIG),
    QEVT_INITIALIZER(Q_INIT_SIG)
};



enum {


    QHSM_MAX_NEST_DEPTH_ = 6
};


#define QHSM_RESERVED_EVT_(state_,sig_) \
    ((*(state_))(me, &QEvt_reserved_[(sig_)]))


#define QS_STATE_ENTRY_(state_,qs_id_) \
    QS_CRIT_ENTRY(); \
    QS_MEM_SYS(); \
    QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, (qs_id_)) \
        QS_OBJ_PRE_(me); \
        QS_FUN_PRE_(state_); \
    QS_END_PRE_() \
    QS_MEM_APP(); \
    QS_CRIT_EXIT()


#define QS_STATE_EXIT_(state_,qs_id_) \
    QS_CRIT_ENTRY(); \
    QS_MEM_SYS(); \
    QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, (qs_id_)) \
        QS_OBJ_PRE_(me); \
        QS_FUN_PRE_(state_); \
    QS_END_PRE_() \
    QS_MEM_APP(); \
    QS_CRIT_EXIT()
# 122 "qf.c"
void QHsm_ctor(QHsm * const me,
    QStateHandler const initial)
{
    static struct QAsmVtable const vtable = {
        &QHsm_init_,
        &QHsm_dispatch_
    #ifdef Q_SPY
        ,&QHsm_getStateHandler_
    #endif
    };

    me->super.vptr = &vtable;
    me->super.state.fun = Q_STATE_CAST(&QHsm_top);
    me->super.temp.fun = initial;
}



void QHsm_init_(
    QAsm * const me,
    void const * const e,
    uint_fast8_t const qs_id)
{
    QF_CRIT_STAT

    #ifdef Q_SPY
    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    if ((QS_priv_.flags & 0x01U) == 0U) {
        QS_priv_.flags |= 0x01U;
        QS_FUN_DICTIONARY(&QHsm_top);
    }
    QS_MEM_APP();
    QS_CRIT_EXIT();
    #else
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler t = me->state.fun;

    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(200, (me->vptr != (struct QAsmVtable *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (t == Q_STATE_CAST(&QHsm_top)));
    QF_CRIT_EXIT();


    QState r = (*me->temp.fun)(me, Q_EVT_CAST(QEvt));

    QF_CRIT_ENTRY();

    Q_ASSERT_INCRIT(210, r == Q_RET_TRAN);

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(t);
        QS_FUN_PRE_(me->temp.fun);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();


    do {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_];
        int_fast8_t ip = 0;

        path[0] = me->temp.fun;
        (void)QHSM_RESERVED_EVT_(me->temp.fun, Q_EMPTY_SIG);
        while (me->temp.fun != t) {
            ++ip;
            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(220, ip < QHSM_MAX_NEST_DEPTH_);
            QF_CRIT_EXIT();
            path[ip] = me->temp.fun;
            (void)QHSM_RESERVED_EVT_(me->temp.fun, Q_EMPTY_SIG);
        }
        me->temp.fun = path[0];


        do {

            if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                == Q_RET_HANDLED)
            {
                QS_STATE_ENTRY_(path[ip], qs_id);
            }
            --ip;
        } while (ip >= 0);

        t = path[0];

        r = QHSM_RESERVED_EVT_(t, Q_INIT_SIG);

    #ifdef Q_SPY
        if (r == Q_RET_TRAN) {
            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(t);
                QS_FUN_PRE_(me->temp.fun);
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();
        }
    #endif

    } while (r == Q_RET_TRAN);

    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_INIT_TRAN, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(t);
    QS_END_PRE_()
    QS_MEM_APP();
    QS_CRIT_EXIT();

    me->state.fun = t;
    #ifndef Q_UNSAFE
    me->temp.uint = ~me->state.uint;
    #endif
}



void QHsm_dispatch_(
    QAsm * const me,
    QEvt const * const e,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler s = me->state.fun;
    QStateHandler t = s;
    QF_CRIT_STAT

    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (s != Q_STATE_CAST(0))
        && (me->state.uint == (uintptr_t)(~me->temp.uint)));
    Q_REQUIRE_INCRIT(302, QEvt_verify_(e));

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_DISPATCH, qs_id)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(s);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();


    QState r;
    me->temp.fun = s;
    do {
        s = me->temp.fun;
        r = (*s)(me, e);

        if (r == Q_RET_UNHANDLED) {

            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_UNHANDLED, qs_id)
                QS_SIG_PRE_(e->sig);
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(s);
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();

            r = QHSM_RESERVED_EVT_(s, Q_EMPTY_SIG);
        }
    } while (r == Q_RET_SUPER);

    if (r >= Q_RET_TRAN) {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_];

        path[0] = me->temp.fun;
        path[1] = t;
        path[2] = s;


        for (; t != s; t = me->temp.fun) {

            if (QHSM_RESERVED_EVT_(t, Q_EXIT_SIG) == Q_RET_HANDLED) {
                QS_STATE_EXIT_(t, qs_id);

                (void)QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG);
            }
        }
        int_fast8_t ip = QHsm_tran_(me, path, qs_id);

    #ifdef Q_SPY
        if (r == Q_RET_TRAN_HIST) {
            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_TRAN_HIST, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(t);
                QS_FUN_PRE_(path[0]);
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();
        }
    #endif


        for (; ip >= 0; --ip) {

            if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                == Q_RET_HANDLED)
            {
                QS_STATE_ENTRY_(path[ip], qs_id);
            }
        }
        t = path[0];
        me->temp.fun = t;


        while (QHSM_RESERVED_EVT_(t, Q_INIT_SIG) == Q_RET_TRAN) {

            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(t);
                QS_FUN_PRE_(me->temp.fun);
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();

            ip = 0;
            path[0] = me->temp.fun;


            (void)QHSM_RESERVED_EVT_(me->temp.fun, Q_EMPTY_SIG);

            while (me->temp.fun != t) {
                ++ip;
                path[ip] = me->temp.fun;

                (void)QHSM_RESERVED_EVT_(me->temp.fun, Q_EMPTY_SIG);
            }
            me->temp.fun = path[0];


            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(410, ip < QHSM_MAX_NEST_DEPTH_);
            QF_CRIT_EXIT();


            do {

                if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                    == Q_RET_HANDLED)
                {
                    QS_STATE_ENTRY_(path[ip], qs_id);
                }
                --ip;
            } while (ip >= 0);

            t = path[0];
        }

        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_TRAN, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(s);
            QS_FUN_PRE_(t);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }

    #ifdef Q_SPY
    else if (r == Q_RET_HANDLED) {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_INTERN_TRAN, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(s);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    else {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_IGNORED, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(me->state.fun);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    #endif

    me->state.fun = t;
    #ifndef Q_UNSAFE
    me->temp.uint = ~me->state.uint;
    #endif
}


#ifdef Q_SPY

QStateHandler QHsm_getStateHandler_(QAsm * const me) {
    return me->state.fun;
}
#endif



bool QHsm_isIn(QHsm * const me,
    QStateHandler const state)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(602, me->super.state.uint
                      == (uintptr_t)(~me->super.temp.uint));
    QF_CRIT_EXIT();

    bool inState = false;


    QState r;
    do {

        if (me->super.temp.fun == state) {
            inState = true;
            r = Q_RET_IGNORED;
        }
        else {
            r = QHSM_RESERVED_EVT_(me->super.temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED);

    #ifndef Q_UNSAFE
    me->super.temp.uint = ~me->super.state.uint;
    #endif

    return inState;
}



QStateHandler QHsm_childState(QHsm * const me,
    QStateHandler const parent)
{
    QStateHandler child = me->super.state.fun;
    bool isFound = false;


    me->super.temp.fun = child;
    QState r;
    do {

        if (me->super.temp.fun == parent) {
            isFound = true;
            r = Q_RET_IGNORED;
        }
        else {
            child = me->super.temp.fun;
            r = QHSM_RESERVED_EVT_(me->super.temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED);

    #ifndef Q_UNSAFE
    me->super.temp.uint = ~me->super.state.uint;
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(890, isFound);
    QF_CRIT_EXIT();

    return child;
}



int_fast8_t QHsm_tran_(
    QAsm * const me,
    QStateHandler * const path,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    int_fast8_t ip = -1;
    QStateHandler t = path[0];
    QStateHandler const s = path[2];
    QF_CRIT_STAT


    if (s == t) {

        if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
            QS_STATE_EXIT_(s, qs_id);
        }
        ip = 0;
    }
    else {

        (void)QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG);

        t = me->temp.fun;


        if (s == t) {
            ip = 0;
        }
        else {

            (void)QHSM_RESERVED_EVT_(s, Q_EMPTY_SIG);


            if (me->temp.fun == t) {

                if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
                    QS_STATE_EXIT_(s, qs_id);
                }
                ip = 0;
            }
            else {

                if (me->temp.fun == path[0]) {

                    if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
                        QS_STATE_EXIT_(s, qs_id);
                    }
                }
                else {


                    int_fast8_t iq = 0;
                    ip = 1;
                    path[1] = t;
                    t = me->temp.fun;


                    QState r = QHSM_RESERVED_EVT_(path[1], Q_EMPTY_SIG);
                    while (r == Q_RET_SUPER) {
                        ++ip;
                        path[ip] = me->temp.fun;
                        if (me->temp.fun == s) {
                            iq = 1;


                            QF_CRIT_ENTRY();
                            Q_ASSERT_INCRIT(510, ip < QHSM_MAX_NEST_DEPTH_);
                            QF_CRIT_EXIT();
                            --ip;
                            r = Q_RET_HANDLED;
                        }
                        else {
                            r = QHSM_RESERVED_EVT_(me->temp.fun, Q_EMPTY_SIG);
                        }
                    }


                    if (iq == 0) {

                        QF_CRIT_ENTRY();
                        Q_ASSERT_INCRIT(520, ip < QHSM_MAX_NEST_DEPTH_);
                        QF_CRIT_EXIT();


                        if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG)
                            == Q_RET_HANDLED)
                        {
                            QS_STATE_EXIT_(s, qs_id);
                        }



                        iq = ip;
                        r = Q_RET_IGNORED;
                        do {
                            if (t == path[iq]) {
                                r = Q_RET_HANDLED;
                                ip = iq - 1;
                                iq = -1;
                            }
                            else {
                                --iq;
                            }
                        } while (iq >= 0);


                        if (r != Q_RET_HANDLED) {


                            r = Q_RET_IGNORED;
                            do {

                                if (QHSM_RESERVED_EVT_(t, Q_EXIT_SIG)
                                    == Q_RET_HANDLED)
                                {
                                    QS_STATE_EXIT_(t, qs_id);

                                    (void)QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG);
                                }
                                t = me->temp.fun;
                                iq = ip;
                                do {

                                    if (t == path[iq]) {
                                        ip = iq - 1;
                                        iq = -1;
                                        r = Q_RET_HANDLED;
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= 0);
                            } while (r != Q_RET_HANDLED);
                        }
                    }
                }
            }
        }
    }
    return ip;
}



QState QHsm_top(QHsm const * const me,
    QEvt const * const e)
{
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(e);
    return Q_RET_IGNORED;
}
# 715 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif




Q_DEFINE_THIS_MODULE("qep_msm")


static struct QMState const l_msm_top_s = {
    (struct QMState *)0,
    Q_STATE_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0)
};

enum {

    QMSM_MAX_ENTRY_DEPTH_ = 4
};






#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
# 761 "qf.c"
void QMsm_ctor(QMsm * const me,
    QStateHandler const initial)
{
    static struct QAsmVtable const vtable = {
        &QMsm_init_,
        &QMsm_dispatch_
    #ifdef Q_SPY
        ,&QMsm_getStateHandler_
    #endif
    };

    me->super.vptr = &vtable;
    me->super.state.obj = &l_msm_top_s;
    me->super.temp.fun = initial;
}



void QMsm_init_(
    QAsm * const me,
    void const * const e,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(200, (me->vptr != (struct QAsmVtable *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (me->state.obj == &l_msm_top_s));
    QF_CRIT_EXIT();


    QState r = (*me->temp.fun)(me, Q_EVT_CAST(QEvt));

    QF_CRIT_ENTRY();

    Q_ASSERT_INCRIT(210, r == Q_RET_TRAN_INIT);

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(me->state.obj->stateHandler);
        QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();


    me->state.obj = me->temp.tatbl->target;


    do {

        r = QMsm_execTatbl_(me, me->temp.tatbl, qs_id);
    } while (r >= Q_RET_TRAN_INIT);

    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_INIT_TRAN, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(me->state.obj->stateHandler);
    QS_END_PRE_()
    QS_MEM_APP();
    QS_CRIT_EXIT();

    #ifndef Q_UNSAFE
    me->temp.uint = ~me->state.uint;
    #endif
}



void QMsm_dispatch_(
    QAsm * const me,
    QEvt const * const e,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QMState const *s = me->state.obj;
    QMState const *t = s;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (s != (QMState *)0)
        && (me->state.uint == (uintptr_t)(~me->temp.uint)));
    Q_REQUIRE_INCRIT(302, QEvt_verify_(e));

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_DISPATCH, qs_id)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(s->stateHandler);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();


    QState r;
    do {
        r = (*t->stateHandler)(me, e);


        if (r >= Q_RET_HANDLED) {
            break;
        }

        else if (r == Q_RET_SUPER) {
            t = t->superstate;
        }

        else if (r == Q_RET_SUPER_SUB) {
            t = me->temp.obj;
        }
        else {
            QF_CRIT_ENTRY();

            Q_ASSERT_INCRIT(310, r == Q_RET_UNHANDLED);

            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_UNHANDLED, qs_id)
                QS_SIG_PRE_(e->sig);
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(t->stateHandler);
            QS_END_PRE_()
            QS_MEM_APP();

            QF_CRIT_EXIT();

            t = t->superstate;
        }
    } while (t != (QMState *)0);

    if (r >= Q_RET_TRAN) {
    #ifdef Q_SPY
        QMState const * const ts = t;

        QF_CRIT_ENTRY();

        Q_ASSERT_INCRIT(320, ts != (QMState *)0);
        QF_CRIT_EXIT();
    #endif

        do {

            struct QMTranActTable const * const tatbl = me->temp.tatbl;
            union QAsmAttr tmp;


            if (r <= Q_RET_TRAN_EP) {
                me->temp.obj = (QMState *)0;
                QMsm_exitToTranSource_(me, s, t, qs_id);
                r = QMsm_execTatbl_(me, tatbl, qs_id);
                s = me->state.obj;
            }

            else if (r == Q_RET_TRAN_HIST) {
                tmp.obj = me->state.obj;
                me->state.obj = s;
                QMsm_exitToTranSource_(me, s, t, qs_id);
                (void)QMsm_execTatbl_(me, tatbl, qs_id);
                r = QMsm_enterHistory_(me, tmp.obj, qs_id);
                s = me->state.obj;
            }
            else {
                QF_CRIT_ENTRY();

                Q_ASSERT_INCRIT(340, r == Q_RET_TRAN_XP);
                QF_CRIT_EXIT();

                tmp.act = me->state.act;
                me->state.obj = s;
                r = (*tmp.act)(me);
                if (r == Q_RET_TRAN) {
    #ifdef Q_SPY
                    tmp.tatbl = me->temp.tatbl;
    #endif
                    QMsm_exitToTranSource_(me, s, t, qs_id);

                    (void)QMsm_execTatbl_(me, tatbl, qs_id);
                    s = me->state.obj;
    #ifdef Q_SPY
                    me->temp.tatbl = tmp.tatbl;
    #endif
                }
                else if (r == Q_RET_TRAN_HIST) {
                    tmp.obj = me->state.obj;
                    me->state.obj = s;
                    s = me->temp.obj;
                    QMsm_exitToTranSource_(me, me->state.obj, t, qs_id);

                    (void)QMsm_execTatbl_(me, tatbl, qs_id);
    #ifdef Q_SPY
                    me->temp.obj = s;
    #endif
                    s = me->state.obj;
                    me->state.obj = tmp.obj;
                }
                else {
                    QF_CRIT_ENTRY();

                    Q_ASSERT_INCRIT(330, r < Q_RET_TRAN);
                    QF_CRIT_EXIT();
                }
            }

            t = s;

        } while (r >= Q_RET_TRAN);

        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_TRAN, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(ts->stateHandler);
            QS_FUN_PRE_(s->stateHandler);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }

    #ifdef Q_SPY

    else if (r == Q_RET_HANDLED) {
        QF_CRIT_ENTRY();

        Q_ASSERT_INCRIT(340, t != (QMState *)0);

        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_INTERN_TRAN, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(t->stateHandler);
        QS_END_PRE_()
        QS_MEM_APP();

        QF_CRIT_EXIT();
    }

    else if (t == (QMState *)0) {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_IGNORED, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(s->stateHandler);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    #endif
    else {

    }

    #ifndef Q_UNSAFE
    me->temp.uint = ~me->state.uint;
    #endif
}



bool QMsm_isInState(QMsm const * const me,
    QMState const * const stateObj)
{
    bool inState = false;

    for (QMState const *s = me->super.state.obj;
         s != (QMState *)0;
         s = s->superstate)
    {
        if (s == stateObj) {
            inState = true;
            break;
        }
    }
    return inState;
}



QMState const * QMsm_childStateObj(QMsm const * const me,
    QMState const * const parent)
{
    QMState const *child = me->super.state.obj;
    bool isFound = false;
    QMState const *s;

    for (s = me->super.state.obj; s != (QMState *)0; s = s->superstate) {
        if (s == parent) {
            isFound = true;
            break;
        }
        else {
            child = s;
        }
    }
    if (!isFound) {
        for (s = me->super.temp.obj; s != (QMState *)0; s = s->superstate) {
            if (s == parent) {
                isFound = true;
                break;
            }
            else {
                child = s;
            }
        }
    }

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(890, isFound);
    QF_CRIT_EXIT();

    return child;
}



QState QMsm_execTatbl_(
    QAsm * const me,
    QMTranActTable const * const tatbl,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();


    Q_REQUIRE_INCRIT(400, tatbl != (struct QMTranActTable *)0);
    QF_CRIT_EXIT();

    QState r = Q_RET_NULL;
    for (QActionHandler const *a = &tatbl->act[0];
         *a != Q_ACTION_CAST(0);
         ++a)
    {
        r = (*(*a))(me);
    #ifdef Q_SPY
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        if (r == Q_RET_ENTRY) {
            QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(me->temp.obj->stateHandler);
            QS_END_PRE_()
        }
        else if (r == Q_RET_EXIT) {
            QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(me->temp.obj->stateHandler);
            QS_END_PRE_()
        }
        else if (r == Q_RET_TRAN_INIT) {
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(tatbl->target->stateHandler);
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);
            QS_END_PRE_()
        }
        else if (r == Q_RET_TRAN_EP) {
            QS_BEGIN_PRE_(QS_QEP_TRAN_EP, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(tatbl->target->stateHandler);
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);
            QS_END_PRE_()
        }
        else if (r == Q_RET_TRAN_XP) {
            QS_BEGIN_PRE_(QS_QEP_TRAN_XP, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(tatbl->target->stateHandler);
                QS_FUN_PRE_(me->temp.tatbl->target->stateHandler);
            QS_END_PRE_()
        }
        else {

        }
        QS_MEM_APP();
        QS_CRIT_EXIT();
    #endif
    }

    me->state.obj = (r >= Q_RET_TRAN)
        ? me->temp.tatbl->target
        : tatbl->target;
    return r;
}



void QMsm_exitToTranSource_(
    QAsm * const me,
    QMState const * const cs,
    QMState const * const ts,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT


    QMState const *s = cs;
    while (s != ts) {

        if (s->exitAction != Q_ACTION_CAST(0)) {

            (void)(*s->exitAction)(me);

            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, qs_id)
                QS_OBJ_PRE_(me);
                QS_FUN_PRE_(s->stateHandler);
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();
        }

        s = s->superstate;

        if (s == (QMState *)0) {
            s = me->temp.obj;
            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(510, s != (QMState *)0);
            QF_CRIT_EXIT();
        }
    }
}



QState QMsm_enterHistory_(
    QAsm * const me,
    QMState const *const hist,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QMState const *s = hist;
    QMState const *ts = me->state.obj;
    QMState const *epath[QMSM_MAX_ENTRY_DEPTH_];

    QF_CRIT_STAT

    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_TRAN_HIST, qs_id)
        QS_OBJ_PRE_(me);
        QS_FUN_PRE_(ts->stateHandler);
        QS_FUN_PRE_(hist->stateHandler);
    QS_END_PRE_()
    QS_MEM_APP();
    QS_CRIT_EXIT();

    int_fast8_t i = 0;
    while (s != ts) {
        if (s->entryAction != Q_ACTION_CAST(0)) {
            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(620, i < QMSM_MAX_ENTRY_DEPTH_);
            QF_CRIT_EXIT();
            epath[i] = s;
            ++i;
        }
        s = s->superstate;
        if (s == (QMState *)0) {
            ts = s;
        }
    }


    while (i > 0) {
        --i;
        (void)(*epath[i]->entryAction)(me);

        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, qs_id)
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(epath[i]->stateHandler);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }

    me->state.obj = hist;


    QState r;
    if (hist->initAction != Q_ACTION_CAST(0)) {
        r = (*hist->initAction)(me);
    }
    else {
        r = Q_RET_NULL;
    }

    return r;
}
# 1321 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif





#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif



QActive * QActive_registry_[QF_MAX_ACTIVE + 1U];





QF_Attr QF_priv_;



void QF_bzero_(
    void * const start,
    uint_fast16_t const len)
{
    uint8_t *ptr = (uint8_t *)start;
    for (uint_fast16_t n = len; n > 0U; --n) {
        *ptr = 0U;
        ++ptr;
    }
}





#ifndef QF_LOG2
uint_fast8_t QF_LOG2(QPSetBits const bitmask) {
    static uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    uint_fast8_t n = 0U;
    QPSetBits tmp;
    QPSetBits x = bitmask;

    #if (QF_MAX_ACTIVE > 16U)
    tmp = (x >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x = tmp;
    }
    #endif
    #if (QF_MAX_ACTIVE > 8U)
    tmp = (x >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x = tmp;
    }
    #endif
    tmp = (x >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x = tmp;
    }
    return n + log2LUT[x];
}
#endif
# 1442 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_actq")




#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif






bool QActive_post_(QActive * const me,
    QEvt const * const e,
    uint_fast16_t const margin,
    void const * const sender)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(sender);
    #endif

    #ifdef Q_UTEST
    #if Q_UTEST != 0
    if (me->super.temp.fun == Q_STATE_CAST(0)) {
        return QActiveDummy_fakePost_(me, e, margin, sender);
    }
    #endif
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(102, QEvt_verify_(e));

    QEQueueCtr nFree = me->eQueue.nFree;


    QS_TEST_PROBE_DEF(&QActive_post_)
    QS_TEST_PROBE_ID(1,
        nFree = 0U;
    )

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true;
        }
        else {
            status = false;
            Q_ERROR_INCRIT(190);
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true;
    }
    else {
        status = false;
    }


    if (QEvt_getPoolId_(e) != 0U) {
        QEvt_refCtr_inc_(e);
    }

    if (status) {

        --nFree;
        me->eQueue.nFree = nFree;
        if (me->eQueue.nMin > nFree) {
            me->eQueue.nMin = nFree;
        }

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(sender);
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_EQC_PRE_(nFree);
            QS_EQC_PRE_(me->eQueue.nMin);
        QS_END_PRE_()

    #ifdef Q_UTEST



        if (QS_LOC_CHECK_(me->prio)) {
            QS_onTestPost(sender, me, e, status);
        }
    #endif

        if (me->eQueue.frontEvt == (QEvt *)0) {
            me->eQueue.frontEvt = e;

    #ifdef QXK_H_
            if (me->super.state.act == Q_ACTION_CAST(0)) {
                QXTHREAD_EQUEUE_SIGNAL_(me);
            }
            else {
                QACTIVE_EQUEUE_SIGNAL_(me);
            }
    #else
            QACTIVE_EQUEUE_SIGNAL_(me);
    #endif
        }

        else {

            me->eQueue.ring[me->eQueue.head] = e;

            if (me->eQueue.head == 0U) {
                me->eQueue.head = me->eQueue.end;
            }
            --me->eQueue.head;
        }

        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(sender);
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_EQC_PRE_(nFree);
            QS_EQC_PRE_(margin);
        QS_END_PRE_()

    #ifdef Q_UTEST



        if (QS_LOC_CHECK_(me->prio)) {
            QS_onTestPost(sender, me, e, status);
        }
    #endif

        QF_MEM_APP();
        QF_CRIT_EXIT();

    #if (QF_MAX_EPOOL > 0U)
        QF_gc(e);
    #endif
    }

    return status;
}





void QActive_postLIFO_(QActive * const me,
    QEvt const * const e)
{
    #ifdef Q_UTEST
    #if Q_UTEST != 0
    if (me->super.temp.fun == Q_STATE_CAST(0)) {
        QActiveDummy_fakePostLIFO_(me, e);
        return;
    }
    #endif
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(202, QEvt_verify_(e));

    #ifdef QXK_H_
    Q_REQUIRE_INCRIT(200, me->super.state.act != Q_ACTION_CAST(0));
    #endif

    QEQueueCtr nFree = me->eQueue.nFree;


    QS_TEST_PROBE_DEF(&QActive_postLIFO_)
    QS_TEST_PROBE_ID(1,
        nFree = 0U;
    )

    Q_REQUIRE_INCRIT(201, nFree != 0U);

    if (QEvt_getPoolId_(e) != 0U) {
        QEvt_refCtr_inc_(e);
    }

    --nFree;
    me->eQueue.nFree = nFree;
    if (me->eQueue.nMin > nFree) {
        me->eQueue.nMin = nFree;
    }

    QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_OBJ_PRE_(me);
        QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
        QS_EQC_PRE_(nFree);
        QS_EQC_PRE_(me->eQueue.nMin);
    QS_END_PRE_()

    #ifdef Q_UTEST



    if (QS_LOC_CHECK_(me->prio)) {
        QS_onTestPost((QActive *)0, me, e, true);
    }
    #endif

    QEvt const * const frontEvt = me->eQueue.frontEvt;
    me->eQueue.frontEvt = e;

    if (frontEvt == (QEvt *)0) {
        QACTIVE_EQUEUE_SIGNAL_(me);
    }
    else {
        ++me->eQueue.tail;
        if (me->eQueue.tail == me->eQueue.end) {
            me->eQueue.tail = 0U;
        }

        me->eQueue.ring[me->eQueue.tail] = frontEvt;
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();
}





QEvt const * QActive_get_(QActive * const me) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QACTIVE_EQUEUE_WAIT_(me);


    QEvt const * const e = me->eQueue.frontEvt;
    QEQueueCtr const nFree = me->eQueue.nFree + 1U;
    me->eQueue.nFree = nFree;

    if (nFree <= me->eQueue.end) {

        me->eQueue.frontEvt = me->eQueue.ring[me->eQueue.tail];
        if (me->eQueue.tail == 0U) {
            me->eQueue.tail = me->eQueue.end;
        }
        --me->eQueue.tail;

        QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_EQC_PRE_(nFree);
        QS_END_PRE_()
    }
    else {
        me->eQueue.frontEvt = (QEvt *)0;


        Q_ASSERT_INCRIT(310, nFree == (me->eQueue.end + 1U));

        QS_BEGIN_PRE_(QS_QF_ACTIVE_GET_LAST, me->prio)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
        QS_END_PRE_()
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return e;
}






uint_fast16_t QF_getQueueMin(uint_fast8_t const prio) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(400, (prio <= QF_MAX_ACTIVE)
                      && (QActive_registry_[prio] != (QActive *)0));
    uint_fast16_t const min =
         (uint_fast16_t)QActive_registry_[prio]->eQueue.nMin;
    QF_CRIT_EXIT();

    return min;
}







void QTicker_ctor(QTicker * const me,
    uint_fast8_t const tickRate)
{
    QActive_ctor(&me->super, Q_STATE_CAST(0));

    static struct QAsmVtable const vtable = {
        &QTicker_init_,
        &QTicker_dispatch_
    #ifdef Q_SPY
        ,&QHsm_getStateHandler_
    #endif
    };
    me->super.super.vptr = &vtable;


    me->super.eQueue.head = (QEQueueCtr)tickRate;
}



void QTicker_init_(
    QAsm * const me,
    void const * const par,
    uint_fast8_t const qs_id)
{
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    Q_UNUSED_PAR(qs_id);

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QACTIVE_CAST_(me)->eQueue.tail = 0U;

    QF_MEM_APP();
    QF_CRIT_EXIT();
}



void QTicker_dispatch_(
    QAsm * const me,
    QEvt const * const e,
    uint_fast8_t const qs_id)
{
    Q_UNUSED_PAR(e);
    Q_UNUSED_PAR(qs_id);

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QEQueueCtr nTicks = QACTIVE_CAST_(me)->eQueue.tail;
    QACTIVE_CAST_(me)->eQueue.tail = 0U;

    QF_MEM_APP();
    QF_CRIT_EXIT();

    for (; nTicks > 0U; --nTicks) {
        QTimeEvt_tick_((uint_fast8_t)QACTIVE_CAST_(me)->eQueue.head, me);
    }
}



void QTicker_trig_(
    QActive * const me,
    void const * const sender)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(sender);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    if (me->eQueue.frontEvt == (QEvt *)0) {

        static QEvt const tickEvt = QEVT_INITIALIZER(0);
        me->eQueue.frontEvt = &tickEvt;
        --me->eQueue.nFree;

        QACTIVE_EQUEUE_SIGNAL_(me);
    }

    ++me->eQueue.tail;

    QS_BEGIN_PRE_(QS_QF_ACTIVE_POST, me->prio)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(sender);
        QS_SIG_PRE_(0U);
        QS_OBJ_PRE_(me);
        QS_2U8_PRE_(0U, 0U);
        QS_EQC_PRE_(0U);
        QS_EQC_PRE_(0U);
    QS_END_PRE_()

    QF_MEM_APP();
    QF_CRIT_EXIT();
}
# 1910 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_defer")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif






bool QActive_defer(QActive const * const me,
    struct QEQueue * const eq,
    QEvt const * const e)
{
    bool const status = QEQueue_post(eq, e, 0U, me->prio);

    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QF_ACTIVE_DEFER, me->prio)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_OBJ_PRE_(eq);
        QS_SIG_PRE_(e->sig);
        QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
    QS_END_PRE_()
    QS_MEM_APP();
    QS_CRIT_EXIT();

    return status;
}





bool QActive_recall(QActive * const me,
    struct QEQueue * const eq)
{
    QEvt const * const e = QEQueue_get(eq, me->prio);
    QF_CRIT_STAT

    bool recalled;
    if (e != (QEvt *)0) {
        QACTIVE_POST_LIFO(me, e);

        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        if (QEvt_getPoolId_(e) != 0U) {





            Q_ASSERT_INCRIT(210, e->refCtr_ >= 2U);



            QEvt_refCtr_dec_(e);
        }

        QS_BEGIN_PRE_(QS_QF_ACTIVE_RECALL, me->prio)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_OBJ_PRE_(eq);
            QS_SIG_PRE_(e->sig);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
        QS_END_PRE_()

        QF_MEM_APP();
        QF_CRIT_EXIT();

        recalled = true;
    }
    else {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();

        QS_BEGIN_PRE_(QS_QF_ACTIVE_RECALL_ATTEMPT, me->prio)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_OBJ_PRE_(eq);
        QS_END_PRE_()

        QS_MEM_APP();
        QS_CRIT_EXIT();

        recalled = false;
    }
    return recalled;
}





uint_fast16_t QActive_flushDeferred(QActive const * const me,
    struct QEQueue * const eq)
{
    uint_fast16_t n = 0U;
    for (QEvt const *e = QEQueue_get(eq, me->prio);
         e != (QEvt *)0;
         e = QEQueue_get(eq, me->prio))
    {
        ++n;
    #if (QF_MAX_EPOOL > 0U)
        QF_gc(e);
    #endif
    }
    return n;
}
# 2079 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

#if (QF_MAX_EPOOL > 0U)

Q_DEFINE_THIS_MODULE("qf_dyn")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif






void QF_poolInit(
    void * const poolSto,
    uint_fast32_t const poolSize,
    uint_fast16_t const evtSize)
{
    uint_fast8_t const poolId = QF_priv_.maxPool_;


    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, poolId < QF_MAX_EPOOL);
    if (poolId > 0U) {
        Q_REQUIRE_INCRIT(201,
            QF_EPOOL_EVENT_SIZE_(QF_priv_.ePool_[poolId - 1U]) < evtSize);
    }
    QF_priv_.maxPool_ = poolId + 1U;

    QF_MEM_APP();
    QF_CRIT_EXIT();


    QF_EPOOL_INIT_(QF_priv_.ePool_[poolId], poolSto, poolSize, evtSize);

    #ifdef Q_SPY

    {
        uint8_t obj_name[9] = "EvtPool?";
        obj_name[7] = (uint8_t)((uint8_t)'0' + poolId + 1U);
        QF_CRIT_ENTRY();
        QF_MEM_SYS();
        QS_obj_dict_pre_(&QF_priv_.ePool_[poolId], (char const *)obj_name);
        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
    #endif
}



uint_fast16_t QF_poolGetMaxBlockSize(void) {
    return QF_EPOOL_EVENT_SIZE_(QF_priv_.ePool_[QF_priv_.maxPool_ - 1U]);
}



uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(400, (poolId <= QF_MAX_EPOOL)
                      && (0U < poolId) && (poolId <= QF_priv_.maxPool_));

    uint_fast16_t const min = (uint_fast16_t)QF_priv_.ePool_[poolId - 1U].nMin;

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return min;
}



QEvt * QF_newX_(
    uint_fast16_t const evtSize,
    uint_fast16_t const margin,
    enum_t const sig)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();


    uint_fast8_t poolId = 0U;
    for (; poolId < QF_priv_.maxPool_; ++poolId) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_priv_.ePool_[poolId])) {
            break;
        }
    }



    Q_REQUIRE_INCRIT(300, poolId < QF_priv_.maxPool_);

    ++poolId;

    QF_MEM_APP();
    QF_CRIT_EXIT();


    QEvt *e;
    #ifdef Q_SPY
    QF_EPOOL_GET_(QF_priv_.ePool_[poolId - 1U], e,
                  ((margin != QF_NO_MARGIN) ? margin : 0U),
                  (uint_fast8_t)QS_EP_ID + poolId);
    #else
    QF_EPOOL_GET_(QF_priv_.ePool_[poolId - 1U], e,
                  ((margin != QF_NO_MARGIN) ? margin : 0U), 0U);
    #endif

    if (e != (QEvt *)0) {
        e->sig = (QSignal)sig;
        e->refCtr_ = 0U;
        e->evtTag_ = (uint8_t)(QEVT_MARKER | poolId);

        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QF_NEW,
                (uint_fast8_t)QS_EP_ID + poolId)
            QS_TIME_PRE_();
            QS_EVS_PRE_(evtSize);
            QS_SIG_PRE_(sig);
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    else {

        QF_CRIT_ENTRY();



        Q_ASSERT_INCRIT(320, margin != QF_NO_MARGIN);

        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QF_NEW_ATTEMPT,
                (uint_fast8_t)QS_EP_ID + poolId)
            QS_TIME_PRE_();
            QS_EVS_PRE_(evtSize);
            QS_SIG_PRE_(sig);
        QS_END_PRE_()
        QS_MEM_APP();

        QF_CRIT_EXIT();
    }



    return e;
}



void QF_gc(QEvt const * const e) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(402, QEvt_verify_(e));

    uint_fast8_t const poolId = QEvt_getPoolId_(e);

    if (poolId != 0U) {
        QF_MEM_SYS();

        if (e->refCtr_ > 1U) {

            QS_BEGIN_PRE_(QS_QF_GC_ATTEMPT,
                    (uint_fast8_t)QS_EP_ID + poolId)
                QS_TIME_PRE_();
                QS_SIG_PRE_(e->sig);
                QS_2U8_PRE_(poolId, e->refCtr_);
            QS_END_PRE_()

            QEvt_refCtr_dec_(e);

            QF_MEM_APP();
            QF_CRIT_EXIT();
        }
        else {

            QS_BEGIN_PRE_(QS_QF_GC,
                    (uint_fast8_t)QS_EP_ID + poolId)
                QS_TIME_PRE_();
                QS_SIG_PRE_(e->sig);
                QS_2U8_PRE_(poolId, e->refCtr_);
            QS_END_PRE_()


            Q_ASSERT_INCRIT(410, (poolId <= QF_priv_.maxPool_)
                                  && (poolId <= QF_MAX_EPOOL));
            QF_MEM_APP();
            QF_CRIT_EXIT();


    #ifdef Q_SPY
            QF_EPOOL_PUT_(QF_priv_.ePool_[poolId - 1U],
                (QEvt *)e,
                (uint_fast8_t)QS_EP_ID + poolId);
    #else
            QF_EPOOL_PUT_(QF_priv_.ePool_[poolId - 1U],
                (QEvt *)e, 0U);
    #endif
        }
    }
    else {
        QF_CRIT_EXIT();
    }
}



QEvt const * QF_newRef_(
    QEvt const * const e,
    void const * const evtRef)
{
    #ifdef Q_UNSAFE
    Q_UNUSED_PAR(evtRef);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    Q_REQUIRE_INCRIT(502, QEvt_verify_(e));

    uint_fast8_t const poolId = QEvt_getPoolId_(e);

    Q_REQUIRE_INCRIT(500, (poolId != 0U)
        && (evtRef == (void *)0));

    QEvt_refCtr_inc_(e);

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QF_NEW_REF,
            (uint_fast8_t)QS_EP_ID + poolId)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_2U8_PRE_(poolId, e->refCtr_);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();

    return e;
}



void QF_deleteRef_(void const * const evtRef) {
    QEvt const * const e = evtRef;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(602, QEvt_verify_(e));

    #ifdef Q_SPY
    uint_fast8_t const poolId = QEvt_getPoolId_(e);
    #endif

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QF_DELETE_REF,
            (uint_fast8_t)QS_EP_ID + poolId)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_2U8_PRE_(poolId, e->refCtr_);
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();

    #if (QF_MAX_EPOOL > 0U)
    QF_gc(e);
    #endif
}


#endif
# 2414 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_mem")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
# 2440 "qf.c"
void QMPool_init(QMPool * const me,
    void * const poolSto,
    uint_fast32_t const poolSize,
    uint_fast16_t const blockSize)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(100, (poolSto != (void *)0)
            && (poolSize >= (uint_fast32_t)sizeof(QFreeBlock))
            && ((uint_fast16_t)(blockSize + sizeof(QFreeBlock)) > blockSize));

    me->free_head = (QFreeBlock *)poolSto;


    me->blockSize = (QMPoolSize)sizeof(QFreeBlock);
    uint_fast16_t nblocks = 1U;
    while (me->blockSize < (QMPoolSize)blockSize) {
        me->blockSize += (QMPoolSize)sizeof(QFreeBlock);
        ++nblocks;
    }


    Q_ASSERT_INCRIT(110, poolSize >= me->blockSize);


    QFreeBlock *fb = me->free_head;
    me->nTot = 1U;


    for (uint_fast32_t size = poolSize - me->blockSize;
         size >= (uint_fast32_t)me->blockSize;
         size -= (uint_fast32_t)me->blockSize)
    {
        fb->next = &fb[nblocks];
    #ifndef Q_UNSAFE
        fb->next_dis = (uintptr_t)(~Q_UINTPTR_CAST_(fb->next));
    #endif
        fb = fb->next;
        ++me->nTot;
    }

    fb->next = (QFreeBlock *)0;
    #ifndef Q_UNSAFE
    fb->next_dis = (uintptr_t)(~Q_UINTPTR_CAST_(fb->next));
    #endif

    me->nFree = me->nTot;
    me->nMin = me->nTot;
    me->start = poolSto;
    me->end = fb;

    QF_MEM_APP();
    QF_CRIT_EXIT();
}



void * QMPool_get(QMPool * const me,
    uint_fast16_t const margin,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();


    QFreeBlock *fb;
    if (me->nFree > (QMPoolCtr)margin) {
        fb = me->free_head;


        Q_ASSERT_INCRIT(300, fb != (QFreeBlock *)0);

        QFreeBlock * const fb_next = fb->next;


        Q_ASSERT_INCRIT(302, Q_UINTPTR_CAST_(fb_next)
                              == (uintptr_t)~fb->next_dis);

        --me->nFree;
        if (me->nFree == 0U) {

            Q_ASSERT_INCRIT(320, fb_next == (QFreeBlock *)0);

            me->nMin = 0U;
        }
        else {







            Q_ASSERT_INCRIT(330,
                (me->start <= fb_next) && (fb_next <= me->end));


            if (me->nMin > me->nFree) {
                me->nMin = me->nFree;
            }
        }

        me->free_head = fb_next;

        QS_BEGIN_PRE_(QS_QF_MPOOL_GET, qs_id)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_MPC_PRE_(me->nFree);
            QS_MPC_PRE_(me->nMin);
        QS_END_PRE_()
    }
    else {
        fb = (QFreeBlock *)0;

        QS_BEGIN_PRE_(QS_QF_MPOOL_GET_ATTEMPT, qs_id)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_MPC_PRE_(me->nFree);
            QS_MPC_PRE_(margin);
        QS_END_PRE_()
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return fb;
}



void QMPool_put(QMPool * const me,
    void * const block,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QFreeBlock * const fb = (QFreeBlock *)block;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, (me->nFree < me->nTot)
                           && (me->start <= fb) && (fb <= me->end));

    fb->next = me->free_head;
    #ifndef Q_UNSAFE
    fb->next_dis = (uintptr_t)(~Q_UINTPTR_CAST_(fb->next));
    #endif


    me->free_head = block;

    ++me->nFree;

    QS_BEGIN_PRE_(QS_QF_MPOOL_PUT, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_MPC_PRE_(me->nFree);
    QS_END_PRE_()

    QF_MEM_APP();
    QF_CRIT_EXIT();
}
# 2656 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_ps")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif



QSubscrList * QActive_subscrList_;



enum_t QActive_maxPubSignal_;






void QActive_psInit(
    QSubscrList * const subscrSto,
    enum_t const maxSignal)
{
    QActive_subscrList_ = subscrSto;
    QActive_maxPubSignal_ = maxSignal;


    for (enum_t sig = 0; sig < maxSignal; ++sig) {
        QPSet_setEmpty(&subscrSto[sig].set);
    #ifndef Q_UNSAFE
        QPSet_update_(&subscrSto[sig].set, &subscrSto[sig].set_dis);
    #endif
    }
}






void QActive_publish_(
    QEvt const * const e,
    void const * const sender,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(qs_id);
    #endif

    QSignal const sig = e->sig;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, sig < (QSignal)QActive_maxPubSignal_);
    Q_REQUIRE_INCRIT(202,
        QPSet_verify_(&QActive_subscrList_[sig].set,
                      &QActive_subscrList_[sig].set_dis));

    QS_BEGIN_PRE_(QS_QF_PUBLISH, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(sender);
        QS_SIG_PRE_(sig);
        QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
    QS_END_PRE_()


    if (QEvt_getPoolId_(e) != 0U) {






        QEvt_refCtr_inc_(e);
    }


    QPSet subscrSet = QActive_subscrList_[sig].set;

    QF_MEM_APP();
    QF_CRIT_EXIT();

    if (QPSet_notEmpty(&subscrSet)) {

        uint_fast8_t p = QPSet_findMax(&subscrSet);

        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        QActive *a = QActive_registry_[p];

        Q_ASSERT_INCRIT(210, a != (QActive *)0);

        QF_MEM_APP();
        QF_CRIT_EXIT();

        QF_SCHED_STAT_
        QF_SCHED_LOCK_(p);
        uint_fast8_t limit = QF_MAX_ACTIVE + 1U;
        do {
            --limit;


            QACTIVE_POST(a, e, sender);

            QPSet_remove(&subscrSet, p);
            if (QPSet_notEmpty(&subscrSet)) {
                p = QPSet_findMax(&subscrSet);

                QF_CRIT_ENTRY();
                QF_MEM_SYS();

                a = QActive_registry_[p];

                Q_ASSERT_INCRIT(220, a != (QActive *)0);

                QF_MEM_APP();
                QF_CRIT_EXIT();
            }
            else {
                p = 0U;
            }
        } while ((p != 0U) && (limit > 0U));

        QF_CRIT_ENTRY();
        Q_ENSURE_INCRIT(290, p == 0U);
        QF_CRIT_EXIT();

        QF_SCHED_UNLOCK_();
    }




    #if (QF_MAX_EPOOL > 0U)
    QF_gc(e);
    #endif
}






void QActive_subscribe(QActive const * const me,
    enum_t const sig)
{
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(300, ((enum_t)Q_USER_SIG <= sig)
        && (sig < QActive_maxPubSignal_)
        && (0U < p) && (p <= QF_MAX_ACTIVE)
        && (QActive_registry_[p] == me));
    Q_REQUIRE_INCRIT(302,
        QPSet_verify_(&QActive_subscrList_[sig].set,
                      &QActive_subscrList_[sig].set_dis));

    QS_BEGIN_PRE_(QS_QF_ACTIVE_SUBSCRIBE, p)
        QS_TIME_PRE_();
        QS_SIG_PRE_(sig);
        QS_OBJ_PRE_(me);
    QS_END_PRE_()


    QPSet_insert(&QActive_subscrList_[sig].set, p);
    #ifndef Q_UNSAFE
    QPSet_update_(&QActive_subscrList_[sig].set,
                  &QActive_subscrList_[sig].set_dis);
    #endif

    QF_MEM_APP();
    QF_CRIT_EXIT();
}






void QActive_unsubscribe(QActive const * const me,
    enum_t const sig)
{
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(400, ((enum_t)Q_USER_SIG <= sig)
        && (sig < QActive_maxPubSignal_)
        && (0U < p) && (p <= QF_MAX_ACTIVE)
        && (QActive_registry_[p] == me));
    Q_REQUIRE_INCRIT(402,
        QPSet_verify_(&QActive_subscrList_[sig].set,
                      &QActive_subscrList_[sig].set_dis));

    QS_BEGIN_PRE_(QS_QF_ACTIVE_UNSUBSCRIBE, p)
        QS_TIME_PRE_();
        QS_SIG_PRE_(sig);
        QS_OBJ_PRE_(me);
    QS_END_PRE_()


    QPSet_remove(&QActive_subscrList_[sig].set, p);
    #ifndef Q_UNSAFE
    QPSet_update_(&QActive_subscrList_[sig].set,
                  &QActive_subscrList_[sig].set_dis);
    #endif

    QF_MEM_APP();
    QF_CRIT_EXIT();
}






void QActive_unsubscribeAll(QActive const * const me) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    uint_fast8_t const p = (uint_fast8_t)me->prio;

    Q_REQUIRE_INCRIT(500, (0U < p) && (p <= QF_MAX_ACTIVE)
                           && (QActive_registry_[p] == me));
    enum_t const maxPubSig = QActive_maxPubSignal_;

    QF_MEM_APP();
    QF_CRIT_EXIT();

    for (enum_t sig = (enum_t)Q_USER_SIG; sig < maxPubSig; ++sig) {
        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        if (QPSet_hasElement(&QActive_subscrList_[sig].set, p)) {
            QPSet_remove(&QActive_subscrList_[sig].set, p);
    #ifndef Q_UNSAFE
            QPSet_update_(&QActive_subscrList_[sig].set,
                          &QActive_subscrList_[sig].set_dis);
    #endif
            QS_BEGIN_PRE_(QS_QF_ACTIVE_UNSUBSCRIBE, p)
                QS_TIME_PRE_();
                QS_SIG_PRE_(sig);
                QS_OBJ_PRE_(me);
            QS_END_PRE_()
        }
        QF_MEM_APP();
        QF_CRIT_EXIT();

        QF_CRIT_EXIT_NOP();
    }
}
# 2973 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_qact")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif






void QActive_ctor(QActive * const me,
    QStateHandler const initial)
{



    QF_bzero_(me, sizeof(*me));




    QHsm_ctor((QHsm *)(me), initial);




    static struct QAsmVtable const vtable = {
        &QHsm_init_,
        &QHsm_dispatch_
    #ifdef Q_SPY
        ,&QHsm_getStateHandler_
    #endif
    };
    me->super.vptr = &vtable;
}






void QActive_register_(QActive * const me) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    if (me->pthre == 0U) {
        me->pthre = me->prio;
    }

    #ifndef Q_UNSAFE

    Q_REQUIRE_INCRIT(100, (0U < me->prio) && (me->prio <= QF_MAX_ACTIVE)
                      && (QActive_registry_[me->prio] == (QActive *)0)
                      && (me->prio <= me->pthre));

    uint8_t prev_thre = me->pthre;
    uint8_t next_thre = me->pthre;

    uint_fast8_t p;
    for (p = (uint_fast8_t)me->prio - 1U; p > 0U; --p) {
        if (QActive_registry_[p] != (QActive *)0) {
            prev_thre = QActive_registry_[p]->pthre;
            break;
        }
    }
    for (p = (uint_fast8_t)me->prio + 1U; p <= QF_MAX_ACTIVE; ++p) {
        if (QActive_registry_[p] != (QActive *)0) {
            next_thre = QActive_registry_[p]->pthre;
            break;
        }
    }

    Q_ASSERT_INCRIT(190, (prev_thre <= me->pthre)
                          && (me->pthre <= next_thre));
    #endif


    QActive_registry_[me->prio] = me;

    QF_MEM_APP();
    QF_CRIT_EXIT();
}






void QActive_unregister_(QActive * const me) {
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, (0U < p) && (p <= QF_MAX_ACTIVE)
                      && (QActive_registry_[p] == me));
    QActive_registry_[p] = (QActive *)0;
    me->super.state.fun = Q_STATE_CAST(0);

    QF_MEM_APP();
    QF_CRIT_EXIT();
}
# 3134 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_qeq")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif





QEQueue QEQueue_dummy;



void QEQueue_init(QEQueue * const me,
    struct QEvt const ** const qSto,
    uint_fast16_t const qLen)
{
    me->frontEvt = (QEvt *)0;
    me->ring = qSto;
    me->end = (QEQueueCtr)qLen;
    if (qLen != 0U) {
        me->head = 0U;
        me->tail = 0U;
    }
    me->nFree = (QEQueueCtr)(qLen + 1U);
    me->nMin = me->nFree;
}



bool QEQueue_post(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast16_t const margin,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, e != (QEvt *)0);

    QEQueueCtr nFree = me->nFree;


    bool status;
    if (((margin == QF_NO_MARGIN) && (nFree > 0U))
        || (nFree > (QEQueueCtr)margin))
    {

        if (QEvt_getPoolId_(e) != 0U) {
            QEvt_refCtr_inc_(e);
        }

        --nFree;
        me->nFree = nFree;
        if (me->nMin > nFree) {
            me->nMin = nFree;
        }

        QS_BEGIN_PRE_(QS_QF_EQUEUE_POST, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_EQC_PRE_(nFree);
            QS_EQC_PRE_(me->nMin);
        QS_END_PRE_()

        if (me->frontEvt == (QEvt *)0) {
            me->frontEvt = e;
        }
        else {

            me->ring[me->head] = e;

            if (me->head == 0U) {
                me->head = me->end;
            }
            --me->head;
        }
        status = true;
    }
    else {

        Q_ASSERT_INCRIT(210, margin != QF_NO_MARGIN);

        QS_BEGIN_PRE_(QS_QF_EQUEUE_POST_ATTEMPT, qs_id)
            QS_TIME_PRE_();
            QS_SIG_PRE_(e->sig);
            QS_OBJ_PRE_(me);
            QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_EQC_PRE_(nFree);
            QS_EQC_PRE_(margin);
        QS_END_PRE_()

        status = false;
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return status;
}



void QEQueue_postLIFO(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QEQueueCtr nFree = me->nFree;

    Q_REQUIRE_INCRIT(300, nFree != 0U);

    if (QEvt_getPoolId_(e) != 0U) {
        QEvt_refCtr_inc_(e);
    }

    --nFree;
    me->nFree = nFree;
    if (me->nMin > nFree) {
        me->nMin = nFree;
    }

    QS_BEGIN_PRE_(QS_QF_EQUEUE_POST_LIFO, qs_id)
        QS_TIME_PRE_();
        QS_SIG_PRE_(e->sig);
        QS_OBJ_PRE_(me);
        QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
        QS_EQC_PRE_(nFree);
        QS_EQC_PRE_(me->nMin);
    QS_END_PRE_()

    QEvt const * const frontEvt = me->frontEvt;
    me->frontEvt = e;

    if (frontEvt != (QEvt *)0) {
        ++me->tail;
        if (me->tail == me->end) {
            me->tail = 0U;
        }
        me->ring[me->tail] = frontEvt;
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();
}



struct QEvt const * QEQueue_get(QEQueue * const me,
    uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QEvt const * const e = me->frontEvt;

    if (e != (QEvt *)0) {

        QEQueueCtr const nFree = me->nFree + 1U;
        me->nFree = nFree;


        if (nFree <= me->end) {
            me->frontEvt = me->ring[me->tail];
            if (me->tail == 0U) {
                me->tail = me->end;
            }
            --me->tail;

            QS_BEGIN_PRE_(QS_QF_EQUEUE_GET, qs_id)
                QS_TIME_PRE_();
                QS_SIG_PRE_(e->sig);
                QS_OBJ_PRE_(me);
                QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
                QS_EQC_PRE_(nFree);
            QS_END_PRE_()
        }
        else {
            me->frontEvt = (QEvt *)0;


            Q_ASSERT_INCRIT(410, nFree == (me->end + 1U));

            QS_BEGIN_PRE_(QS_QF_EQUEUE_GET_LAST, qs_id)
                QS_TIME_PRE_();
                QS_SIG_PRE_(e->sig);
                QS_OBJ_PRE_(me);
                QS_2U8_PRE_(QEvt_getPoolId_(e), e->refCtr_);
            QS_END_PRE_()
        }
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return e;
}
# 3407 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif





#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
# 3433 "qf.c"
void QMActive_ctor(QMActive * const me,
    QStateHandler const initial)
{



    QF_bzero_(me, sizeof(*me));




    QMsm_ctor((QMsm *)(me), initial);




    static struct QAsmVtable const vtable = {
        &QMsm_init_,
        &QMsm_dispatch_
    #ifdef Q_SPY
        ,&QMsm_getStateHandler_
    #endif
    };
    me->super.super.vptr = &vtable;
}
# 3501 "qf.c"
#define QP_IMPL 
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"
#ifdef Q_SPY
    #include "qs_port.h"
    #include "qs_pkg.h"
#else
    #include "qs_dummy.h"
#endif

Q_DEFINE_THIS_MODULE("qf_time")



#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif





QTimeEvt QTimeEvt_timeEvtHead_[QF_MAX_TICK_RATE];



void QTimeEvt_ctorX(QTimeEvt * const me,
    QActive * const act,
    enum_t const sig,
    uint_fast8_t const tickRate)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (sig != 0)
        && (tickRate < QF_MAX_TICK_RATE));
    QF_CRIT_EXIT();





    me->super.sig = (QSignal)sig;
    me->super.refCtr_ = (uint8_t)tickRate;
    me->super.evtTag_ = QEVT_MARKER;

    me->next = (QTimeEvt *)0;
    me->act = act;
    me->ctr = 0U;
    me->interval = 0U;
}



void QTimeEvt_armX(QTimeEvt * const me,
    QTimeEvtCtr const nTicks,
    QTimeEvtCtr const interval)
{
    uint_fast8_t const tickRate
                       = ((uint_fast8_t)me->super.refCtr_ & QTE_TICK_RATE);
    QTimeEvtCtr const ctr = me->ctr;
    #ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(400, (me->act != (void *)0)
        && (ctr == 0U)
        && (nTicks != 0U)
        && (tickRate < (uint_fast8_t)QF_MAX_TICK_RATE)
        && (me->super.sig >= (QSignal)Q_USER_SIG));
    #ifdef Q_UNSAFE
    Q_UNUSED_PAR(ctr);
    #endif

    me->ctr = nTicks;
    me->interval = interval;





    if ((me->super.refCtr_ & QTE_IS_LINKED) == 0U) {

        me->super.refCtr_ |= QTE_IS_LINKED;







        me->next = (QTimeEvt *)QTimeEvt_timeEvtHead_[tickRate].act;
        QTimeEvt_timeEvtHead_[tickRate].act = me;
    }

    QS_BEGIN_PRE_(QS_QF_TIMEEVT_ARM, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_OBJ_PRE_(me->act);
        QS_TEC_PRE_(nTicks);
        QS_TEC_PRE_(interval);
        QS_U8_PRE_(tickRate);
    QS_END_PRE_()

    QF_MEM_APP();
    QF_CRIT_EXIT();
}



bool QTimeEvt_disarm(QTimeEvt * const me) {
    #ifdef Q_SPY
    uint_fast8_t const qs_id = QACTIVE_CAST_(me->act)->prio;
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();


    bool wasArmed;
    if (me->ctr != 0U) {
        wasArmed = true;
        me->super.refCtr_ |= QTE_WAS_DISARMED;

        QS_BEGIN_PRE_(QS_QF_TIMEEVT_DISARM, qs_id)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_OBJ_PRE_(me->act);
            QS_TEC_PRE_(me->ctr);
            QS_TEC_PRE_(me->interval);
            QS_U8_PRE_(me->super.refCtr_ & QTE_TICK_RATE);
        QS_END_PRE_()

        me->ctr = 0U;
    }
    else {
        wasArmed = false;
        me->super.refCtr_ &= (uint8_t)(~QTE_WAS_DISARMED & 0xFFU);

        QS_BEGIN_PRE_(QS_QF_TIMEEVT_DISARM_ATTEMPT, qs_id)
            QS_TIME_PRE_();
            QS_OBJ_PRE_(me);
            QS_OBJ_PRE_(me->act);
            QS_U8_PRE_(me->super.refCtr_ & QTE_TICK_RATE);
        QS_END_PRE_()
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return wasArmed;
}



bool QTimeEvt_rearm(QTimeEvt * const me,
    QTimeEvtCtr const nTicks)
{
    uint_fast8_t const tickRate
                       = (uint_fast8_t)me->super.refCtr_ & QTE_TICK_RATE;
    #ifdef Q_SPY
    uint_fast8_t const qs_id = ((QActive *)(me->act))->prio;
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(600, (me->act != (void *)0)
        && (tickRate < QF_MAX_TICK_RATE)
        && (nTicks != 0U)
        && (me->super.sig >= (QSignal)Q_USER_SIG));


    bool wasArmed;
    if (me->ctr == 0U) {
        wasArmed = false;







        if ((me->super.refCtr_ & QTE_IS_LINKED) == 0U) {

            me->super.refCtr_ |= QTE_IS_LINKED;
# 3701 "qf.c"
            me->next = (QTimeEvt *)QTimeEvt_timeEvtHead_[tickRate].act;
            QTimeEvt_timeEvtHead_[tickRate].act = me;
        }
    }
    else {
        wasArmed = true;
    }
    me->ctr = nTicks;

    QS_BEGIN_PRE_(QS_QF_TIMEEVT_REARM, qs_id)
        QS_TIME_PRE_();
        QS_OBJ_PRE_(me);
        QS_OBJ_PRE_(me->act);
        QS_TEC_PRE_(me->ctr);
        QS_TEC_PRE_(me->interval);
        QS_2U8_PRE_(tickRate, (wasArmed ? 1U : 0U));
    QS_END_PRE_()

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return wasArmed;
}



bool QTimeEvt_wasDisarmed(QTimeEvt * const me) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    uint8_t const wasDisarmed = (me->super.refCtr_ & QTE_WAS_DISARMED);
    me->super.refCtr_ |= QTE_WAS_DISARMED;

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return wasDisarmed != 0U;
}



QTimeEvtCtr QTimeEvt_currCtr(QTimeEvt const * const me) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QTimeEvtCtr const ctr = me->ctr;
    QF_CRIT_EXIT();

    return ctr;
}



void QTimeEvt_tick_(
    uint_fast8_t const tickRate,
    void const * const sender)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(sender);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(100, tickRate < Q_DIM(QTimeEvt_timeEvtHead_));

    QTimeEvt *prev = &QTimeEvt_timeEvtHead_[tickRate];

    QS_BEGIN_PRE_(QS_QF_TICK, 0U)
        ++prev->ctr;
        QS_TEC_PRE_(prev->ctr);
        QS_U8_PRE_(tickRate);
    QS_END_PRE_()


    uint_fast8_t limit = 2U*QF_MAX_ACTIVE;
    for (; limit > 0U; --limit) {
        QTimeEvt *e = prev->next;

        if (e == (QTimeEvt *)0) {


            if (QTimeEvt_timeEvtHead_[tickRate].act != (void *)0) {


                Q_ASSERT_INCRIT(110, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QTimeEvt_timeEvtHead_[tickRate].act;
                QTimeEvt_timeEvtHead_[tickRate].act = (void *)0;
                e = prev->next;
            }
            else {
                break;
            }
        }


        Q_ASSERT_INCRIT(112, QEvt_verify_(Q_EVT_CAST(QEvt)));

        if (e->ctr == 0U) {
            prev->next = e->next;

            e->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED & 0xFFU);

            QF_MEM_APP();
            QF_CRIT_EXIT();
# 3816 "qf.c"
            QF_CRIT_EXIT_NOP();
        }
        else {
            --e->ctr;

            if (e->ctr == 0U) {
                QActive * const act = (QActive *)e->act;

                if (e->interval != 0U) {
                    e->ctr = e->interval;
                    prev = e;
                }
                else {
                    prev->next = e->next;


                    e->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED & 0xFFU);


                    QS_BEGIN_PRE_(QS_QF_TIMEEVT_AUTO_DISARM, act->prio)
                        QS_OBJ_PRE_(e);
                        QS_OBJ_PRE_(act);
                        QS_U8_PRE_(tickRate);
                    QS_END_PRE_()
                }

                QS_BEGIN_PRE_(QS_QF_TIMEEVT_POST, act->prio)
                    QS_TIME_PRE_();
                    QS_OBJ_PRE_(e);
                    QS_SIG_PRE_(e->super.sig);
                    QS_OBJ_PRE_(act);
                    QS_U8_PRE_(tickRate);
                QS_END_PRE_()

    #ifdef QXK_H_
                if (e->super.sig < Q_USER_SIG) {
                    QXThread_timeout_(act);
                }
                else {
                    QF_MEM_APP();
                    QF_CRIT_EXIT();


                    QACTIVE_POST(act, &e->super, sender);
                }
    #else
                QF_MEM_APP();
                QF_CRIT_EXIT();


                QACTIVE_POST(act, &e->super, sender);
    #endif
            }
            else {
                prev = e;

                QF_MEM_APP();
                QF_CRIT_EXIT();


                QF_CRIT_EXIT_NOP();
            }
        }
        QF_CRIT_ENTRY();
        QF_MEM_SYS();
    }

    Q_ENSURE_INCRIT(190, limit > 0U);
    QF_MEM_APP();
    QF_CRIT_EXIT();
}



bool QTimeEvt_noActive(uint_fast8_t const tickRate) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(800, tickRate < QF_MAX_TICK_RATE);
    QF_CRIT_EXIT();

    bool inactive;
    if (QTimeEvt_timeEvtHead_[tickRate].next != (QTimeEvt *)0) {
        inactive = false;
    }
    else if ((QTimeEvt_timeEvtHead_[tickRate].act != (void *)0)) {
        inactive = false;
    }
    else {
        inactive = true;
    }
    return inactive;
}
