//****************************************************************************
// Model: dpp.qm
// File:  ./philo.cpp
//
// This code has been generated by QM tool (see state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//****************************************************************************
//${.::philo.cpp} ............................................................
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

// Active object class -------------------------------------------------------
namespace DPP {

//${AOs::Philo} ..............................................................
class Philo : public QP::QMActive {
private:
    QP::QTimeEvt m_timeEvt;

public:
    Philo();

protected:
    static QP::QState initial(Philo * const me, QP::QEvt const * const e);
    static QP::QState thinking  (Philo * const me, QP::QEvt const * const e);
    static QP::QState thinking_e(Philo * const me);
    static QP::QState thinking_x(Philo * const me);
    static QP::QMState const thinking_s;
    static QP::QState hungry  (Philo * const me, QP::QEvt const * const e);
    static QP::QState hungry_e(Philo * const me);
    static QP::QMState const hungry_s;
    static QP::QState eating  (Philo * const me, QP::QEvt const * const e);
    static QP::QState eating_e(Philo * const me);
    static QP::QState eating_x(Philo * const me);
    static QP::QMState const eating_s;
};

} // namespace DPP

namespace DPP {

// Local objects -------------------------------------------------------------
static Philo l_philo[N_PHILO];   // storage for all Philos

// helper function to provide a randomized think time for Philos
inline QP::QTimeEvtCtr think_time() {
    return static_cast<QP::QTimeEvtCtr>((BSP_random() % BSP_TICKS_PER_SEC)
                                        + (BSP_TICKS_PER_SEC/2U));
}

// helper function to provide a randomized eat time for Philos
inline QP::QTimeEvtCtr eat_time() {
    return static_cast<QP::QTimeEvtCtr>((BSP_random() % BSP_TICKS_PER_SEC)
                                        + BSP_TICKS_PER_SEC);
}

// helper function to provide the ID of Philo "me"
inline uint8_t PHILO_ID(Philo const * const me) {
    return static_cast<uint8_t>(me - l_philo);
}

enum InternalSignals {           // internal signals
    TIMEOUT_SIG = MAX_SIG
};

// Global objects ------------------------------------------------------------
QP::QActive * const AO_Philo[N_PHILO] = { // "opaque" pointers to Philo AO
    &l_philo[0],
    &l_philo[1],
    &l_philo[2],
    &l_philo[3],
    &l_philo[4]
};

} // namespace DPP

// Philo definition ----------------------------------------------------------
namespace DPP {

//${AOs::Philo} ..............................................................
//${AOs::Philo::Philo} .......................................................
Philo::Philo()
  : QMActive(Q_STATE_CAST(&Philo::initial)),
    m_timeEvt(this, TIMEOUT_SIG, 0U)
{}

//${AOs::Philo::SM} ..........................................................
QP::QState Philo::initial(Philo * const me, QP::QEvt const * const e) {
    static struct {
        QP::QMState const *target;
        QP::QActionHandler act[2];
    } const tatbl_ = { // transition-action table
        &thinking_s,
        {
            Q_ACTION_CAST(&Philo::thinking_e), // entry
            Q_ACTION_CAST(0)  // zero terminator
        }
    };
    // ${AOs::Philo::SM::initial}
    static bool registered = false; // starts off with 0, per C-standard
    (void)e; // suppress the compiler warning about unused parameter
    if (!registered) {
        registered = true;

        QS_OBJ_DICTIONARY(&l_philo[0]);
        QS_OBJ_DICTIONARY(&l_philo[0].m_timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[1]);
        QS_OBJ_DICTIONARY(&l_philo[1].m_timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[2]);
        QS_OBJ_DICTIONARY(&l_philo[2].m_timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[3]);
        QS_OBJ_DICTIONARY(&l_philo[3].m_timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[4]);
        QS_OBJ_DICTIONARY(&l_philo[4].m_timeEvt);

        QS_FUN_DICTIONARY(&Philo::initial);
        QS_FUN_DICTIONARY(&Philo::thinking);
        QS_FUN_DICTIONARY(&Philo::hungry);
        QS_FUN_DICTIONARY(&Philo::eating);
    }
    QS_SIG_DICTIONARY(HUNGRY_SIG, me);  // signal for each Philos
    QS_SIG_DICTIONARY(TIMEOUT_SIG, me); // signal for each Philos

    me->subscribe(EAT_SIG);
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Philo::SM::thinking} ................................................
QP::QMState const Philo::thinking_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&Philo::thinking),
    Q_ACTION_CAST(&Philo::thinking_e),
    Q_ACTION_CAST(&Philo::thinking_x),
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Philo::SM::thinking}
QP::QState Philo::thinking_e(Philo * const me) {
    me->m_timeEvt.armX(think_time(), 0U);
    return QM_ENTRY(&thinking_s);
}
// ${AOs::Philo::SM::thinking}
QP::QState Philo::thinking_x(Philo * const me) {
    (void)me->m_timeEvt.disarm();
    return QM_EXIT(&thinking_s);
}
// ${AOs::Philo::SM::thinking}
QP::QState Philo::thinking(Philo * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Philo::SM::thinking::TIMEOUT}
        case TIMEOUT_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[3];
            } const tatbl_ = { // transition-action table
                &hungry_s,
                {
                    Q_ACTION_CAST(&Philo::thinking_x), // exit
                    Q_ACTION_CAST(&Philo::hungry_e), // entry
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        // ${AOs::Philo::SM::thinking::EAT, DONE}
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT(Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = QM_HANDLED();
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Philo::SM::hungry} ..................................................
QP::QMState const Philo::hungry_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&Philo::hungry),
    Q_ACTION_CAST(&Philo::hungry_e),
    Q_ACTION_CAST(0), // no exit action
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Philo::SM::hungry}
QP::QState Philo::hungry_e(Philo * const me) {
    TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG);
    pe->philoNum = PHILO_ID(me);
    AO_Table->POST(pe, me);
    return QM_ENTRY(&hungry_s);
}
// ${AOs::Philo::SM::hungry}
QP::QState Philo::hungry(Philo * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Philo::SM::hungry::EAT}
        case EAT_SIG: {
            // ${AOs::Philo::SM::hungry::EAT::[Q_EVT_CAST(Ta~]}
            if (Q_EVT_CAST(TableEvt)->philoNum == PHILO_ID(me)) {
                static struct {
                    QP::QMState const *target;
                    QP::QActionHandler act[2];
                } const tatbl_ = { // transition-action table
                    &eating_s,
                    {
                        Q_ACTION_CAST(&Philo::eating_e), // entry
                        Q_ACTION_CAST(0)  // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        // ${AOs::Philo::SM::hungry::DONE}
        case DONE_SIG: {
            /* DONE must be for other Philos than this one */
            Q_ASSERT(Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = QM_HANDLED();
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//${AOs::Philo::SM::eating} ..................................................
QP::QMState const Philo::eating_s = {
    static_cast<QP::QMState const *>(0), // superstate (top)
    Q_STATE_CAST(&Philo::eating),
    Q_ACTION_CAST(&Philo::eating_e),
    Q_ACTION_CAST(&Philo::eating_x),
    Q_ACTION_CAST(0)  // no intitial tran.
};
// ${AOs::Philo::SM::eating}
QP::QState Philo::eating_e(Philo * const me) {
    me->m_timeEvt.armX(eat_time(), 0U);
    return QM_ENTRY(&eating_s);
}
// ${AOs::Philo::SM::eating}
QP::QState Philo::eating_x(Philo * const me) {
    TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
    pe->philoNum = PHILO_ID(me);
    QP::QF::PUBLISH(pe, me);
    (void)me->m_timeEvt.disarm();
    return QM_EXIT(&eating_s);
}
// ${AOs::Philo::SM::eating}
QP::QState Philo::eating(Philo * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // ${AOs::Philo::SM::eating::TIMEOUT}
        case TIMEOUT_SIG: {
            static struct {
                QP::QMState const *target;
                QP::QActionHandler act[3];
            } const tatbl_ = { // transition-action table
                &thinking_s,
                {
                    Q_ACTION_CAST(&Philo::eating_x), // exit
                    Q_ACTION_CAST(&Philo::thinking_e), // entry
                    Q_ACTION_CAST(0)  // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        // ${AOs::Philo::SM::eating::EAT, DONE}
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT(Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = QM_HANDLED();
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

} // namespace DPP
