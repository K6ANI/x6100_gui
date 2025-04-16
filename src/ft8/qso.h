#pragma once

typedef enum {
    // "CQ CALL ..." or "CQ DX CALL ..." or "CQ EU CALL ..."
    FTX_MSG_TYPE_CQ,
    // "CALL1 CALL2 GRID"
    FTX_MSG_TYPE_GRID,
    // "CALL1 CALL2 +1"
    FTX_MSG_TYPE_REPORT,
    // "CALL1 CALL2 R+1"
    FTX_MSG_TYPE_R_REPORT,
    // "CALL1 CALL2 RR73"
    FTX_MSG_TYPE_RR73,
    // "CALL1 CALL2 73"
    FTX_MSG_TYPE_73,

    FXT_MSG_TYPE_OTHER,
} ftx_msg_type_t;

typedef struct {
    char           grid[9];
    char           call_de[13];
    int            local_snr;
    int            remote_snr;
    bool           to_me;
    float          freq_hz;
    float          time_sec;
    ftx_msg_type_t type;
} ftx_msg_meta_t;

typedef struct {
    char msg[35];
    int  repeats;
} ftx_tx_msg_t;

typedef void (*save_qso_cb_t)(const char *remote_callsign, const char *remote_grid, const int r_snr, const int s_snr);

#ifdef __cplusplus
#include <string>
#include <vector>

std::vector<std::string> split_text(std::string text);

class Candidate {
  public:
    Candidate(std::string remote_callsign);
    void set_grid(std::string grid);
    void set_report(int report);
    void set_msg_type(ftx_msg_type_t msg_type);
    void set_local_snr(int snr);
    void set_rcvd_snr(int snr);
    bool match_callsign(std::string callsign);
    bool is_finished();
    void save_qso(save_qso_cb_t save_qso_cb);

    std::string get_tx_text(std::string local_callsign, std::string local_qth);

  private:
    std::string    _remote_callsign;
    int            _local_snr;
    ftx_msg_type_t _last_rx_type;
    std::string    _grid;
    int            _rcvd_snr;
    int            _sent_snr;
    bool           _saved=false;
};

class FTxQsoProcessor {
  public:
    FTxQsoProcessor(std::string local_callsign, std::string local_qth, save_qso_cb_t save_qso_cb, int max_repeats);
    ~FTxQsoProcessor();

    /// @brief Pass an RXed text, update internal state and meta
    /// @param[in] text received text
    /// @param[in] snr local snr value
    /// @param[out] meta meta information to fill
    /// @param[out] tx_msg TX message to fill
    void add_rx_text(std::string text, const int snr, ftx_msg_meta_t *meta, ftx_tx_msg_t *tx_msg);
    void process_grid(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr, ftx_tx_msg_t &tx_msg);
    void process_report(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr, ftx_tx_msg_t &tx_msg);
    void process_r_report(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr, ftx_tx_msg_t &tx_msg);
    void process_rr73(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr, ftx_tx_msg_t &tx_msg);
    void process_73(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr, ftx_tx_msg_t &tx_msg);
    void process_cq(ftx_msg_meta_t *meta, std::vector<std::string> &tokens, const int snr);
    void set_auto(bool);
    void start_new_slot();
    void reset();
    void start_qso(ftx_msg_meta_t *meta, ftx_tx_msg_t *tx_msg);

  private:
    bool          _auto    = true;
    bool          _have_tx = false;
    std::string   _local_callsign;
    std::string   _local_qth;
    save_qso_cb_t _save_qso_cb;
    int           _max_repeats = -1;

    ftx_msg_type_t _last_rx_type;
    Candidate     *_next_candidate = NULL;
    Candidate     *_cur_candidate  = NULL;

    Candidate **get_candidate_to_update(std::string call_de);
    Candidate  *get_or_create_cur_candidate(std::string remote_callsign);
};
#else
typedef struct FTxQsoProcessor FTxQsoProcessor;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern FTxQsoProcessor *ftx_qso_processor_init(const char *local_callsign, const char *qth, save_qso_cb_t save_qso_cb, int max_repeats);
extern void             ftx_qso_processor_delete(FTxQsoProcessor *p);
extern void ftx_qso_processor_add_rx_text(FTxQsoProcessor *p, const char *text, const int snr, ftx_msg_meta_t *meta,
                                          ftx_tx_msg_t *tx_msg);
extern void ftx_qso_processor_set_auto(FTxQsoProcessor *p, bool);
extern void ftx_qso_processor_start_new_slot(FTxQsoProcessor *p);
extern void ftx_qso_processor_reset(FTxQsoProcessor *p);
extern void ftx_qso_processor_start_qso(FTxQsoProcessor *p, ftx_msg_meta_t *meta, ftx_tx_msg_t *tx_msg);

#ifdef __cplusplus
}
#endif
