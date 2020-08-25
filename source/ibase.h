/*
 *	MODULE:		ibase.h
 *	DESCRIPTION:	OSRI entrypoints and defines
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001
 * 2001.07.28: John Bellardo:  Added blr_skip
 * 2001.09.18: Ann Harrison:   New info codes
 * 17-Oct-2001 Mike Nordell: CPU affinity
 * 2001-04-16 Paul Beach: ISC_TIME_SECONDS_PRECISION_SCALE modified for HP10
 * Compiler Compatibility
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - EPSON, XENIX, MAC (MAC_AUX), Cray and OS/2
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 *
 */

#ifndef JRD_IBASE_H
#define JRD_IBASE_H

constexpr int FB_API_VER  = 20;
constexpr int isc_Version = 4;

constexpr int  ISC_TRUE	 = 1;
constexpr int  ISC_FALSE = 0;
#if !(defined __cplusplus)
#define  ISC__TRUE	ISC_TRUE
#define  ISC__FALSE	ISC_FALSE
#endif

#define ISC_FAR

#if defined _MSC_VER && _MSC_VER >= 1300
#define FB_API_DEPRECATED __declspec(deprecated)
#elif defined __GNUC__ && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2))
#define FB_API_DEPRECATED __attribute__((__deprecated__))
#else
#define FB_API_DEPRECATED
#endif


#ifndef INCLUDE_TYPES_PUB_H
#define INCLUDE_TYPES_PUB_H

#if defined(_LP64) || defined(__LP64__) || defined(__arch64__)
typedef unsigned int    FB_API_HANDLE;
#else
typedef void*           FB_API_HANDLE;
#endif

typedef long ISC_STATUS;

constexpr int ISC_STATUS_LENGTH = 20;
typedef ISC_STATUS ISC_STATUS_ARRAY[ISC_STATUS_LENGTH];

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define  ISC_EXPORT     __stdcall
#define  ISC_EXPORT_VARARG      __cdecl
#else
#define  ISC_EXPORT
#define  ISC_EXPORT_VARARG
#endif

#if defined(_LP64) || defined(__LP64__) || defined(__arch64__)
typedef int             ISC_LONG;
typedef unsigned int    ISC_ULONG;
#else
typedef signed long     ISC_LONG;
typedef unsigned long   ISC_ULONG;
#endif

typedef signed short    ISC_SHORT;
typedef unsigned short  ISC_USHORT;

typedef unsigned char   ISC_UCHAR;
typedef char            ISC_SCHAR;

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__GNUC__)
typedef __int64                         ISC_INT64;
typedef unsigned __int64        ISC_UINT64;
#else
typedef long long int                   ISC_INT64;
typedef unsigned long long int  ISC_UINT64;
#endif

#ifndef ISC_TIMESTAMP_DEFINED
typedef int                     ISC_DATE;
typedef unsigned int    ISC_TIME;
typedef struct
{
    ISC_DATE timestamp_date;
    ISC_TIME timestamp_time;
} ISC_TIMESTAMP;
#define ISC_TIMESTAMP_DEFINED
#endif  

struct GDS_QUAD_t {
    ISC_LONG gds_quad_high;
    ISC_ULONG gds_quad_low;
};

typedef struct GDS_QUAD_t GDS_QUAD;
typedef struct GDS_QUAD_t ISC_QUAD;

#define isc_quad_high   gds_quad_high
#define isc_quad_low    gds_quad_low

#endif 

/********************************/
/* InterBase Handle Definitions */
/********************************/

typedef FB_API_HANDLE isc_att_handle;
typedef FB_API_HANDLE isc_blob_handle;
typedef FB_API_HANDLE isc_db_handle;
typedef FB_API_HANDLE isc_req_handle;
typedef FB_API_HANDLE isc_stmt_handle;
typedef FB_API_HANDLE isc_svc_handle;
typedef FB_API_HANDLE isc_tr_handle;
typedef void (* isc_callback) ();
typedef ISC_LONG isc_resv_handle;

typedef void (*ISC_PRINT_CALLBACK) (void*, ISC_SHORT, const char*);
typedef void (*ISC_VERSION_CALLBACK)(void*, const char*);
typedef void (*ISC_EVENT_CALLBACK)(void*, ISC_USHORT, const ISC_UCHAR*);

/*******************************************************************/
/* Blob id structure                                               */
/*******************************************************************/

#if !(defined __cplusplus)
typedef GDS_QUAD GDS__QUAD;
#endif /* !(defined __cplusplus) */

typedef struct
{
    short array_bound_lower;
    short array_bound_upper;
} ISC_ARRAY_BOUND;

typedef struct
{
    ISC_UCHAR	array_desc_dtype;
    ISC_SCHAR			array_desc_scale;
    unsigned short	array_desc_length;
    ISC_SCHAR			array_desc_field_name[32];
    ISC_SCHAR			array_desc_relation_name[32];
    short			array_desc_dimensions;
    short			array_desc_flags;
    ISC_ARRAY_BOUND	array_desc_bounds[16];
} ISC_ARRAY_DESC;

typedef struct
{
    short			blob_desc_subtype;
    short			blob_desc_charset;
    short			blob_desc_segment_size;
    ISC_UCHAR	blob_desc_field_name[32];
    ISC_UCHAR	blob_desc_relation_name[32];
} ISC_BLOB_DESC;

/***************************/
/* Blob control structure  */
/***************************/

typedef struct isc_blob_ctl
{
    ISC_STATUS	(* ctl_source)();	/* Source filter */
    struct isc_blob_ctl*	ctl_source_handle;	/* Argument to pass to source filter */
    short					ctl_to_sub_type;		/* Target type */
    short					ctl_from_sub_type;		/* Source type */
    unsigned short			ctl_buffer_length;		/* Length of buffer */
    unsigned short			ctl_segment_length;		/* Length of current segment */
    unsigned short			ctl_bpb_length;			/* Length of blob parameter  block */
    /* Internally, this is const UCHAR*, but this public struct probably can't change. */
    ISC_SCHAR*					ctl_bpb;				/* Address of blob parameter block */
    ISC_UCHAR*			ctl_buffer;				/* Address of segment buffer */
    ISC_LONG				ctl_max_segment;		/* Length of longest segment */
    ISC_LONG				ctl_number_segments;	/* Total number of segments */
    ISC_LONG				ctl_total_length;		/* Total length of blob */
    ISC_STATUS*				ctl_status;				/* Address of status vector */
    long					ctl_data[8];			/* Application specific data */
} * ISC_BLOB_CTL;

/***************************/
/* Blob stream definitions */
/***************************/

typedef struct bstream
{
    isc_blob_handle	bstr_blob;		/* Blob handle */
    ISC_SCHAR *			bstr_buffer;	/* Address of buffer */
    ISC_SCHAR *			bstr_ptr;		/* Next character */
    short			bstr_length;	/* Length of buffer */
    short			bstr_cnt;		/* Characters in buffer */
    char			bstr_mode;		/* (mode) ? OUTPUT : INPUT */
} BSTREAM;

/* Three ugly macros, one even using octal radix... sigh... */
#define getb(p)	(--(p)->bstr_cnt >= 0 ? *(p)->bstr_ptr++ & 0377: BLOB_get (p))
#define putb(x,p) (((x) == '\n' || (!(--(p)->bstr_cnt))) ? BLOB_put ((x),p) : ((int) (*(p)->bstr_ptr++ = (unsigned) (x))))
#define putbx(x,p) ((!(--(p)->bstr_cnt)) ? BLOB_put ((x),p) : ((int) (*(p)->bstr_ptr++ = (unsigned) (x))))

/********************************************************************/
/* CVC: Public blob interface definition held in val.h.             */
/* For some unknown reason, it was only documented in langRef       */
/* and being the structure passed by the engine to UDFs it never    */
/* made its way into this public definitions file.                  */
/* Being its original name "blob", I renamed it blobcallback here.  */
/* I did the full definition with the proper parameters instead of  */
/* the weak C declaration with any number and type of parameters.   */
/* Since the first parameter -BLB- is unknown outside the engine,   */
/* it's more accurate to use void* than int* as the blob pointer    */
/********************************************************************/

#if !defined(JRD_VAL_H) && !defined(REQUESTER)
/* Blob passing structure */

/* This enum applies to parameter "mode" in blob_lseek */
enum blob_lseek_mode {blb_seek_relative = 1, blb_seek_from_tail = 2};
/* This enum applies to the value returned by blob_get_segment */
enum blob_get_result {blb_got_fragment = -1, blb_got_eof = 0, blb_got_full_segment = 1};

typedef struct blobcallback {
    short (*blob_get_segment)
    (void* hnd, ISC_UCHAR* buffer, ISC_USHORT buf_size, ISC_USHORT* result_len);
    void*		blob_handle;
    ISC_LONG	blob_number_segments;
    ISC_LONG	blob_max_segment;
    ISC_LONG	blob_total_length;
    void (*blob_put_segment)
    (void* hnd, const ISC_UCHAR* buffer, ISC_USHORT buf_size);
    ISC_LONG (*blob_lseek)
    (void* hnd, ISC_USHORT mode, ISC_LONG offset);
}  *BLOBCALLBACK;
#endif /* !defined(JRD_VAL_H) && !defined(REQUESTER) */


/********************************************************************/
/* CVC: Public descriptor interface held in dsc2.h.                  */
/* We need it documented to be able to recognize NULL in UDFs.      */
/* Being its original name "dsc", I renamed it paramdsc here.       */
/* Notice that I adjust to the original definition: contrary to     */
/* other cases, the typedef is the same struct not the pointer.     */
/* I included the enumeration of dsc_dtype possible values.         */
/* Ultimately, dsc2.h should be part of the public interface.        */
/********************************************************************/

#if !defined(JRD_DSC_H)
/* This is the famous internal descriptor that UDFs can use, too. */
typedef struct paramdsc {
    ISC_UCHAR	dsc_dtype;
    signed char		dsc_scale;
    ISC_USHORT		dsc_length;
    short		dsc_sub_type;
    ISC_USHORT		dsc_flags;
    ISC_UCHAR	*dsc_address;
} PARAMDSC;

#if !defined(JRD_VAL_H)
/* This is a helper struct to work with varchars. */
typedef struct paramvary {
    ISC_USHORT		vary_length;
    ISC_UCHAR		vary_string[1];
} PARAMVARY;
#endif /* !defined(JRD_VAL_H) */


#ifndef JRD_DSC_PUB_H
#define JRD_DSC_PUB_H

constexpr int DSC_null        = 1;
constexpr int DSC_no_subtype  = 2;
constexpr int DSC_nullable    = 4;

constexpr int dtype_unknown   = 0;
constexpr int dtype_text      = 1;
constexpr int dtype_cstring   = 2;
constexpr int dtype_varying   = 3;

constexpr int dtype_packed    = 6;
constexpr int dtype_byte      = 7;
constexpr int dtype_short     = 8;
constexpr int dtype_long      = 9;
constexpr int dtype_quad      = 10;
constexpr int dtype_real      = 11;
constexpr int dtype_double    = 12;
constexpr int dtype_d_float   = 13;
constexpr int dtype_sql_date  = 14;
constexpr int dtype_sql_time  = 15;
constexpr int dtype_timestamp = 16;
constexpr int dtype_blob      = 17;
constexpr int dtype_array     = 18;
constexpr int dtype_int64     = 19;
constexpr int DTYPE_TYPE_MAX  = 20;

constexpr int ISC_TIME_SECONDS_PRECISION       = 10000;
constexpr int ISC_TIME_SECONDS_PRECISION_SCALE = -4;

#endif 


#endif /* !defined(JRD_DSC_H) */

/***************************/
/* Dynamic SQL definitions */
/***************************/


#ifndef DSQL_SQLDA_PUB_H
#define DSQL_SQLDA_PUB_H

constexpr int DSQL_close = 1;
constexpr int DSQL_drop  = 2;

typedef struct
{
    ISC_SHORT       sqltype;
    ISC_SHORT       sqlscale;
    ISC_SHORT       sqlsubtype;
    ISC_SHORT       sqllen;
    ISC_SCHAR*      sqldata;
    ISC_SHORT*      sqlind;
    ISC_SHORT       sqlname_length;
    ISC_SCHAR       sqlname[32];
    ISC_SHORT       relname_length;
    ISC_SCHAR       relname[32];
    ISC_SHORT       ownname_length;
    ISC_SCHAR       ownname[32];
    ISC_SHORT       aliasname_length;
    ISC_SCHAR       aliasname[32];
} XSQLVAR;

constexpr int SQLDA_VERSION = 1;

typedef struct
{
    ISC_SHORT       version;
    ISC_SCHAR       sqldaid[8];
    ISC_LONG        sqldabc;
    ISC_SHORT       sqln;
    ISC_SHORT       sqld;
    XSQLVAR sqlvar[1];
} XSQLDA;

template<typename T1>
constexpr int XSQLDA_LENGTH(T1 n){return (sizeof (XSQLDA) + (n - 1) * sizeof (XSQLVAR));}

constexpr int SQL_TEXT      = 452;
constexpr int SQL_VARYING   = 448;
constexpr int SQL_SHORT     = 500;
constexpr int SQL_LONG      = 496;
constexpr int SQL_FLOAT     = 482;
constexpr int SQL_DOUBLE    = 480;
constexpr int SQL_D_FLOAT   = 530;
constexpr int SQL_TIMESTAMP = 510;
constexpr int SQL_BLOB      = 520;
constexpr int SQL_ARRAY     = 540;
constexpr int SQL_QUAD      = 550;
constexpr int SQL_TYPE_TIME = 560;
constexpr int SQL_TYPE_DATE = 570;
constexpr int SQL_INT64     = 580;

constexpr int SQL_DATE = SQL_TIMESTAMP;

constexpr int SQL_DIALECT_V5            = 1;
constexpr int SQL_DIALECT_V6_TRANSITION = 2;
constexpr int SQL_DIALECT_V6            = 3;
constexpr int SQL_DIALECT_CURRENT       = SQL_DIALECT_V6;

#endif 


/***************************/
/* OSRI database functions */
/***************************/

#ifdef __cplusplus
extern "C" {
#endif

ISC_STATUS ISC_EXPORT isc_attach_database(ISC_STATUS*,
                                          short,
                                          const ISC_SCHAR*,
                                          isc_db_handle*,
                                          short,
                                          const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_array_gen_sdl(ISC_STATUS*,
                                        const ISC_ARRAY_DESC*,
                                        ISC_SHORT*,
                                        ISC_UCHAR*,
                                        ISC_SHORT*);

ISC_STATUS ISC_EXPORT isc_array_get_slice(ISC_STATUS*,
                                          isc_db_handle*,
                                          isc_tr_handle*,
                                          ISC_QUAD*,
                                          const ISC_ARRAY_DESC*,
                                          void*,
                                          ISC_LONG*);

ISC_STATUS ISC_EXPORT isc_array_lookup_bounds(ISC_STATUS*,
                                              isc_db_handle*,
                                              isc_tr_handle*,
                                              const ISC_SCHAR*,
                                              const ISC_SCHAR*,
                                              ISC_ARRAY_DESC*);

ISC_STATUS ISC_EXPORT isc_array_lookup_desc(ISC_STATUS*,
                                            isc_db_handle*,
                                            isc_tr_handle*,
                                            const ISC_SCHAR*,
                                            const ISC_SCHAR*,
                                            ISC_ARRAY_DESC*);

ISC_STATUS ISC_EXPORT isc_array_set_desc(ISC_STATUS*,
                                         const ISC_SCHAR*,
                                         const ISC_SCHAR*,
                                         const short*,
                                         const short*,
                                         const short*,
                                         ISC_ARRAY_DESC*);

ISC_STATUS ISC_EXPORT isc_array_put_slice(ISC_STATUS*,
                                          isc_db_handle*,
                                          isc_tr_handle*,
                                          ISC_QUAD*,
                                          const ISC_ARRAY_DESC*,
                                          void*,
                                          ISC_LONG *);

void ISC_EXPORT isc_blob_default_desc(ISC_BLOB_DESC*,
                                      const ISC_UCHAR*,
                                      const ISC_UCHAR*);

ISC_STATUS ISC_EXPORT isc_blob_gen_bpb(ISC_STATUS*,
                                       const ISC_BLOB_DESC*,
                                       const ISC_BLOB_DESC*,
                                       unsigned short,
                                       ISC_UCHAR*,
                                       unsigned short*);

ISC_STATUS ISC_EXPORT isc_blob_info(ISC_STATUS*,
                                    isc_blob_handle*,
                                    short,
                                    const ISC_SCHAR*,
                                    short,
                                    ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_blob_lookup_desc(ISC_STATUS*,
                                           isc_db_handle*,
                                           isc_tr_handle*,
                                           const ISC_UCHAR*,
                                           const ISC_UCHAR*,
                                           ISC_BLOB_DESC*,
                                           ISC_UCHAR*);

ISC_STATUS ISC_EXPORT isc_blob_set_desc(ISC_STATUS*,
                                        const ISC_UCHAR*,
                                        const ISC_UCHAR*,
                                        short,
                                        short,
                                        short,
                                        ISC_BLOB_DESC*);

ISC_STATUS ISC_EXPORT isc_cancel_blob(ISC_STATUS *,
                                      isc_blob_handle *);

ISC_STATUS ISC_EXPORT isc_cancel_events(ISC_STATUS *,
                                        isc_db_handle *,
                                        ISC_LONG *);

ISC_STATUS ISC_EXPORT isc_close_blob(ISC_STATUS *,
                                     isc_blob_handle *);

ISC_STATUS ISC_EXPORT isc_commit_retaining(ISC_STATUS *,
                                           isc_tr_handle *);

ISC_STATUS ISC_EXPORT isc_commit_transaction(ISC_STATUS *,
                                             isc_tr_handle *);

ISC_STATUS ISC_EXPORT isc_create_blob(ISC_STATUS*,
                                      isc_db_handle*,
                                      isc_tr_handle*,
                                      isc_blob_handle*,
                                      ISC_QUAD*);

ISC_STATUS ISC_EXPORT isc_create_blob2(ISC_STATUS*,
                                       isc_db_handle*,
                                       isc_tr_handle*,
                                       isc_blob_handle*,
                                       ISC_QUAD*,
                                       short,
                                       const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_create_database(ISC_STATUS*,
                                          short,
                                          const ISC_SCHAR*,
                                          isc_db_handle*,
                                          short,
                                          const ISC_SCHAR*,
                                          short);

ISC_STATUS ISC_EXPORT isc_database_info(ISC_STATUS*,
                                        isc_db_handle*,
                                        short,
                                        const ISC_SCHAR*,
                                        short,
                                        ISC_SCHAR*);

void ISC_EXPORT isc_decode_date(const ISC_QUAD*,
                                void*);

void ISC_EXPORT isc_decode_sql_date(const ISC_DATE*,
                                    void*);

void ISC_EXPORT isc_decode_sql_time(const ISC_TIME*,
                                    void*);

void ISC_EXPORT isc_decode_timestamp(const ISC_TIMESTAMP*,
                                     void*);

ISC_STATUS ISC_EXPORT isc_detach_database(ISC_STATUS *,
                                          isc_db_handle *);

ISC_STATUS ISC_EXPORT isc_drop_database(ISC_STATUS *,
                                        isc_db_handle *);

ISC_STATUS ISC_EXPORT isc_dsql_allocate_statement(ISC_STATUS *,
                                                  isc_db_handle *,
                                                  isc_stmt_handle *);

ISC_STATUS ISC_EXPORT isc_dsql_alloc_statement2(ISC_STATUS *,
                                                isc_db_handle *,
                                                isc_stmt_handle *);

ISC_STATUS ISC_EXPORT isc_dsql_describe(ISC_STATUS *,
                                        isc_stmt_handle *,
                                        unsigned short,
                                        XSQLDA *);

ISC_STATUS ISC_EXPORT isc_dsql_describe_bind(ISC_STATUS *,
                                             isc_stmt_handle *,
                                             unsigned short,
                                             XSQLDA *);

ISC_STATUS ISC_EXPORT isc_dsql_exec_immed2(ISC_STATUS*,
                                           isc_db_handle*,
                                           isc_tr_handle*,
                                           unsigned short,
                                           const ISC_SCHAR*,
                                           unsigned short,
                                           XSQLDA*,
                                           XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_execute(ISC_STATUS*,
                                       isc_tr_handle*,
                                       isc_stmt_handle*,
                                       unsigned short,
                                       XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_execute2(ISC_STATUS*,
                                        isc_tr_handle*,
                                        isc_stmt_handle*,
                                        unsigned short,
                                        XSQLDA*,
                                        XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_execute_immediate(ISC_STATUS*,
                                                 isc_db_handle*,
                                                 isc_tr_handle*,
                                                 unsigned short,
                                                 const ISC_SCHAR*,
                                                 unsigned short,
                                                 XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_fetch(ISC_STATUS *,
                                     isc_stmt_handle *,
                                     unsigned short,
                                     XSQLDA *);

ISC_STATUS ISC_EXPORT isc_dsql_finish(isc_db_handle *);

ISC_STATUS ISC_EXPORT isc_dsql_free_statement(ISC_STATUS *,
                                              isc_stmt_handle *,
                                              unsigned short);

ISC_STATUS ISC_EXPORT isc_dsql_insert(ISC_STATUS*,
                                      isc_stmt_handle*,
                                      unsigned short,
                                      XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_prepare(ISC_STATUS*,
                                       isc_tr_handle*,
                                       isc_stmt_handle*,
                                       unsigned short,
                                       const ISC_SCHAR*,
                                       unsigned short,
                                       XSQLDA*);

ISC_STATUS ISC_EXPORT isc_dsql_set_cursor_name(ISC_STATUS*,
                                               isc_stmt_handle*,
                                               const ISC_SCHAR*,
                                               unsigned short);

ISC_STATUS ISC_EXPORT isc_dsql_sql_info(ISC_STATUS*,
                                        isc_stmt_handle*,
                                        short,
                                        const ISC_SCHAR*,
                                        short,
                                        ISC_SCHAR*);

void ISC_EXPORT isc_encode_date(const void*,
                                ISC_QUAD*);

void ISC_EXPORT isc_encode_sql_date(const void*,
                                    ISC_DATE*);

void ISC_EXPORT isc_encode_sql_time(const void*,
                                    ISC_TIME*);

void ISC_EXPORT isc_encode_timestamp(const void*,
                                     ISC_TIMESTAMP*);

ISC_LONG ISC_EXPORT_VARARG isc_event_block(ISC_UCHAR**,
                                           ISC_UCHAR**,
                                           unsigned short, ...);

void ISC_EXPORT isc_event_counts(ISC_ULONG*,
                                 short,
                                 ISC_UCHAR*,
                                 const ISC_UCHAR *);

/* 17 May 2001 - isc_expand_dpb is DEPRECATED */
void FB_API_DEPRECATED ISC_EXPORT_VARARG isc_expand_dpb(ISC_SCHAR**,
                                                        short*, ...);

int ISC_EXPORT isc_modify_dpb(ISC_SCHAR**,
                              short*,
                              unsigned short,
                              const ISC_SCHAR*,
                              short);

ISC_LONG ISC_EXPORT isc_free(ISC_SCHAR *);

ISC_STATUS ISC_EXPORT isc_get_segment(ISC_STATUS *,
                                      isc_blob_handle *,
                                      unsigned short *,
                                      unsigned short,
                                      ISC_SCHAR *);

ISC_STATUS ISC_EXPORT isc_get_slice(ISC_STATUS*,
                                    isc_db_handle*,
                                    isc_tr_handle*,
                                    ISC_QUAD*,
                                    short,
                                    const ISC_SCHAR*,
                                    short,
                                    const ISC_LONG*,
                                    ISC_LONG,
                                    void*,
                                    ISC_LONG*);

/* CVC: This non-const signature is needed for compatibility, see gds.cpp. */
ISC_LONG FB_API_DEPRECATED ISC_EXPORT isc_interprete(ISC_SCHAR*,
                                                     ISC_STATUS**);

/* This const params version used in the engine and other places. */
ISC_LONG ISC_EXPORT fb_interpret(ISC_SCHAR*,
                                 unsigned int,
                                 const ISC_STATUS**);

ISC_STATUS ISC_EXPORT isc_open_blob(ISC_STATUS*,
                                    isc_db_handle*,
                                    isc_tr_handle*,
                                    isc_blob_handle*,
                                    ISC_QUAD*);

ISC_STATUS ISC_EXPORT isc_open_blob2(ISC_STATUS*,
                                     isc_db_handle*,
                                     isc_tr_handle*,
                                     isc_blob_handle*,
                                     ISC_QUAD*,
                                     ISC_USHORT,
                                     const ISC_UCHAR*);

ISC_STATUS ISC_EXPORT isc_prepare_transaction2(ISC_STATUS*,
                                               isc_tr_handle*,
                                               ISC_USHORT,
                                               const ISC_UCHAR*);

void ISC_EXPORT isc_print_sqlerror(ISC_SHORT,
                                   const ISC_STATUS*);

ISC_STATUS ISC_EXPORT isc_print_status(const ISC_STATUS*);

ISC_STATUS ISC_EXPORT isc_put_segment(ISC_STATUS*,
                                      isc_blob_handle*,
                                      unsigned short,
                                      const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_put_slice(ISC_STATUS*,
                                    isc_db_handle*,
                                    isc_tr_handle*,
                                    ISC_QUAD*,
                                    short,
                                    const ISC_SCHAR*,
                                    short,
                                    const ISC_LONG*,
                                    ISC_LONG,
                                    void*);

ISC_STATUS ISC_EXPORT isc_que_events(ISC_STATUS*,
                                     isc_db_handle*,
                                     ISC_LONG*,
                                     short,
                                     const ISC_UCHAR*,
                                     ISC_EVENT_CALLBACK,
                                     void*);

ISC_STATUS ISC_EXPORT isc_rollback_retaining(ISC_STATUS *,
                                             isc_tr_handle *);

ISC_STATUS ISC_EXPORT isc_rollback_transaction(ISC_STATUS *,
                                               isc_tr_handle *);

ISC_STATUS ISC_EXPORT isc_start_multiple(ISC_STATUS *,
                                         isc_tr_handle *,
                                         short,
                                         void *);

ISC_STATUS ISC_EXPORT_VARARG isc_start_transaction(ISC_STATUS *,
                                                   isc_tr_handle *,
                                                   short, ...);

ISC_LONG ISC_EXPORT isc_sqlcode(const ISC_STATUS*);

void ISC_EXPORT isc_sql_interprete(short,
                                   ISC_SCHAR *,
                                   short);

ISC_STATUS ISC_EXPORT isc_transaction_info(ISC_STATUS*,
                                           isc_tr_handle*,
                                           short,
                                           const ISC_SCHAR*,
                                           short,
                                           ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_transact_request(ISC_STATUS*,
                                           isc_db_handle*,
                                           isc_tr_handle*,
                                           unsigned short,
                                           const ISC_SCHAR*,
                                           unsigned short,
                                           ISC_SCHAR*,
                                           unsigned short,
                                           ISC_SCHAR*);

ISC_LONG ISC_EXPORT isc_vax_integer(const ISC_SCHAR*,
                                    short);

ISC_INT64 ISC_EXPORT isc_portable_integer(const ISC_UCHAR*,
                                          short);

/*************************************/
/* Security Functions and structures */
/*************************************/

constexpr int sec_uid_spec		     = 0x01;
constexpr int sec_gid_spec		     = 0x02;
constexpr int sec_server_spec		 = 0x04;
constexpr int sec_password_spec	     = 0x08;
constexpr int sec_group_name_spec	 = 0x10;
constexpr int sec_first_name_spec	 = 0x20;
constexpr int sec_middle_name_spec   = 0x40;
constexpr int sec_last_name_spec	 = 0x80;
constexpr int sec_dba_user_name_spec = 0x100;
constexpr int sec_dba_password_spec  = 0x200;

constexpr int sec_protocol_tcpip   = 1;
constexpr int sec_protocol_netbeui = 2;
constexpr int sec_protocol_spx     = 3; /* -- Deprecated Protocol. Declaration retained for compatibility   */
constexpr int sec_protocol_local   = 4;

typedef struct {
    short sec_flags;			/* which fields are specified */
    int uid;					/* the user's id */
    int gid;					/* the user's group id */
    int protocol;				/* protocol to use for connection */
    ISC_SCHAR *server;				/* server to administer */
    ISC_SCHAR *user_name;			/* the user's name */
    ISC_SCHAR *password;				/* the user's password */
    ISC_SCHAR *group_name;			/* the group name */
    ISC_SCHAR *first_name;			/* the user's first name */
    ISC_SCHAR *middle_name;			/* the user's middle name */
    ISC_SCHAR *last_name;			/* the user's last name */
    ISC_SCHAR *dba_user_name;		/* the dba user name */
    ISC_SCHAR *dba_password;			/* the dba password */
} USER_SEC_DATA;

ISC_STATUS ISC_EXPORT isc_add_user(ISC_STATUS*, const USER_SEC_DATA*);

ISC_STATUS ISC_EXPORT isc_delete_user(ISC_STATUS*, const USER_SEC_DATA*);

ISC_STATUS ISC_EXPORT isc_modify_user(ISC_STATUS*, const USER_SEC_DATA*);

/**********************************/
/*  Other OSRI functions          */
/**********************************/

ISC_STATUS ISC_EXPORT isc_compile_request(ISC_STATUS*,
                                          isc_db_handle*,
                                          isc_req_handle*,
                                          short,
                                          const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_compile_request2(ISC_STATUS*,
                                           isc_db_handle*,
                                           isc_req_handle*,
                                           short,
                                           const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_ddl(ISC_STATUS*,
                              isc_db_handle*,
                              isc_tr_handle*,
                              short,
                              const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_prepare_transaction(ISC_STATUS*,
                                              isc_tr_handle*);


ISC_STATUS ISC_EXPORT isc_receive(ISC_STATUS*,
                                  isc_req_handle*,
                                  short,
                                  short,
                                  void*,
                                  short);

ISC_STATUS ISC_EXPORT isc_reconnect_transaction(ISC_STATUS*,
                                                isc_db_handle*,
                                                isc_tr_handle*,
                                                short,
                                                const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_release_request(ISC_STATUS*,
                                          isc_req_handle*);

ISC_STATUS ISC_EXPORT isc_request_info(ISC_STATUS*,
                                       isc_req_handle*,
                                       short,
                                       short,
                                       const ISC_SCHAR*,
                                       short,
                                       ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_seek_blob(ISC_STATUS*,
                                    isc_blob_handle*,
                                    short,
                                    ISC_LONG,
                                    ISC_LONG*);

ISC_STATUS ISC_EXPORT isc_send(ISC_STATUS*,
                               isc_req_handle*,
                               short,
                               short,
                               const void*,
                               short);

ISC_STATUS ISC_EXPORT isc_start_and_send(ISC_STATUS*,
                                         isc_req_handle*,
                                         isc_tr_handle*,
                                         short,
                                         short,
                                         const void*,
                                         short);

ISC_STATUS ISC_EXPORT isc_start_request(ISC_STATUS *,
                                        isc_req_handle *,
                                        isc_tr_handle *,
                                        short);

ISC_STATUS ISC_EXPORT isc_unwind_request(ISC_STATUS *,
                                         isc_tr_handle *,
                                         short);

ISC_STATUS ISC_EXPORT isc_wait_for_event(ISC_STATUS*,
                                         isc_db_handle*,
                                         short,
                                         const ISC_UCHAR*,
                                         ISC_UCHAR*);


/*****************************/
/* Other Sql functions       */
/*****************************/

ISC_STATUS ISC_EXPORT isc_close(ISC_STATUS*,
                                const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_declare(ISC_STATUS*,
                                  const ISC_SCHAR*,
                                  const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_describe(ISC_STATUS*,
                                   const ISC_SCHAR*,
                                   XSQLDA *);

ISC_STATUS ISC_EXPORT isc_describe_bind(ISC_STATUS*,
                                        const ISC_SCHAR*,
                                        XSQLDA*);

ISC_STATUS ISC_EXPORT isc_execute(ISC_STATUS*,
                                  isc_tr_handle*,
                                  const ISC_SCHAR*,
                                  XSQLDA*);

ISC_STATUS ISC_EXPORT isc_execute_immediate(ISC_STATUS*,
                                            isc_db_handle*,
                                            isc_tr_handle*,
                                            short*,
                                            const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_fetch(ISC_STATUS*,
                                const ISC_SCHAR*,
                                XSQLDA*);

ISC_STATUS ISC_EXPORT isc_open(ISC_STATUS*,
                               isc_tr_handle*,
                               const ISC_SCHAR*,
                               XSQLDA*);

ISC_STATUS ISC_EXPORT isc_prepare(ISC_STATUS*,
                                  isc_db_handle*,
                                  isc_tr_handle*,
                                  const ISC_SCHAR*,
                                  short*,
                                  const ISC_SCHAR*,
                                  XSQLDA*);


/*************************************/
/* Other Dynamic sql functions       */
/*************************************/

ISC_STATUS ISC_EXPORT isc_dsql_execute_m(ISC_STATUS*,
                                         isc_tr_handle*,
                                         isc_stmt_handle*,
                                         unsigned short,
                                         const ISC_SCHAR*,
                                         unsigned short,
                                         unsigned short,
                                         ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_execute2_m(ISC_STATUS*,
                                          isc_tr_handle*,
                                          isc_stmt_handle*,
                                          unsigned short,
                                          const ISC_SCHAR*,
                                          unsigned short,
                                          unsigned short,
                                          const ISC_SCHAR*,
                                          unsigned short,
                                          ISC_SCHAR*,
                                          unsigned short,
                                          unsigned short,
                                          ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_execute_immediate_m(ISC_STATUS*,
                                                   isc_db_handle*,
                                                   isc_tr_handle*,
                                                   unsigned short,
                                                   const ISC_SCHAR*,
                                                   unsigned short,
                                                   unsigned short,
                                                   const ISC_SCHAR*,
                                                   unsigned short,
                                                   unsigned short,
                                                   ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_exec_immed3_m(ISC_STATUS*,
                                             isc_db_handle*,
                                             isc_tr_handle*,
                                             unsigned short,
                                             const ISC_SCHAR*,
                                             unsigned short,
                                             unsigned short,
                                             const ISC_SCHAR*,
                                             unsigned short,
                                             unsigned short,
                                             ISC_SCHAR*,
                                             unsigned short,
                                             ISC_SCHAR*,
                                             unsigned short,
                                             unsigned short,
                                             ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_fetch_m(ISC_STATUS*,
                                       isc_stmt_handle*,
                                       unsigned short,
                                       const ISC_SCHAR*,
                                       unsigned short,
                                       unsigned short,
                                       ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_insert_m(ISC_STATUS*,
                                        isc_stmt_handle*,
                                        unsigned short,
                                        const ISC_SCHAR*,
                                        unsigned short,
                                        unsigned short,
                                        const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_prepare_m(ISC_STATUS*,
                                         isc_tr_handle*,
                                         isc_stmt_handle*,
                                         unsigned short,
                                         const ISC_SCHAR*,
                                         unsigned short,
                                         unsigned short,
                                         const ISC_SCHAR*,
                                         unsigned short,
                                         ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_dsql_release(ISC_STATUS*,
                                       const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_close(ISC_STATUS*,
                                           const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_declare(ISC_STATUS*,
                                             const ISC_SCHAR*,
                                             const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_describe(ISC_STATUS*,
                                              const ISC_SCHAR*,
                                              unsigned short,
                                              XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_describe_bind(ISC_STATUS*,
                                                   const ISC_SCHAR*,
                                                   unsigned short,
                                                   XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_execute(ISC_STATUS*,
                                             isc_tr_handle*,
                                             const ISC_SCHAR*,
                                             unsigned short,
                                             XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_execute2(ISC_STATUS*,
                                              isc_tr_handle*,
                                              const ISC_SCHAR*,
                                              unsigned short,
                                              XSQLDA*,
                                              XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_execute_immed(ISC_STATUS*,
                                                   isc_db_handle*,
                                                   isc_tr_handle*,
                                                   unsigned short,
                                                   const ISC_SCHAR*,
                                                   unsigned short,
                                                   XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_fetch(ISC_STATUS*,
                                           const ISC_SCHAR*,
                                           unsigned short,
                                           XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_open(ISC_STATUS*,
                                          isc_tr_handle*,
                                          const ISC_SCHAR*,
                                          unsigned short,
                                          XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_open2(ISC_STATUS*,
                                           isc_tr_handle*,
                                           const ISC_SCHAR*,
                                           unsigned short,
                                           XSQLDA*,
                                           XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_insert(ISC_STATUS*,
                                            const ISC_SCHAR*,
                                            unsigned short,
                                            XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_prepare(ISC_STATUS*,
                                             isc_db_handle*,
                                             isc_tr_handle*,
                                             const ISC_SCHAR*,
                                             unsigned short,
                                             const ISC_SCHAR*,
                                             unsigned short,
                                             XSQLDA*);

ISC_STATUS ISC_EXPORT isc_embed_dsql_release(ISC_STATUS*,
                                             const ISC_SCHAR*);


/******************************/
/* Other Blob functions       */
/******************************/

BSTREAM* ISC_EXPORT BLOB_open(isc_blob_handle,
                              ISC_SCHAR*,
                              int);

int ISC_EXPORT BLOB_put(ISC_SCHAR,
                        BSTREAM*);

int ISC_EXPORT BLOB_close(BSTREAM*);

int ISC_EXPORT BLOB_get(BSTREAM*);

int ISC_EXPORT BLOB_display(ISC_QUAD*,
                            isc_db_handle,
                            isc_tr_handle,
                            const ISC_SCHAR*);

int ISC_EXPORT BLOB_dump(ISC_QUAD*,
                         isc_db_handle,
                         isc_tr_handle,
                         const ISC_SCHAR*);

int ISC_EXPORT BLOB_edit(ISC_QUAD*,
                         isc_db_handle,
                         isc_tr_handle,
                         const ISC_SCHAR*);

int ISC_EXPORT BLOB_load(ISC_QUAD*,
                         isc_db_handle,
                         isc_tr_handle,
                         const ISC_SCHAR*);

int ISC_EXPORT BLOB_text_dump(ISC_QUAD*,
                              isc_db_handle,
                              isc_tr_handle,
                              const ISC_SCHAR*);

int ISC_EXPORT BLOB_text_load(ISC_QUAD*,
                              isc_db_handle,
                              isc_tr_handle,
                              const ISC_SCHAR*);

BSTREAM* ISC_EXPORT Bopen(ISC_QUAD*,
                          isc_db_handle,
                          isc_tr_handle,
                          const ISC_SCHAR*);

/* Disabled, not found anywhere.
BSTREAM* ISC_EXPORT Bopen2(ISC_QUAD*,
                                   isc_db_handle,
                                   isc_tr_handle,
                                   const ISC_SCHAR*,
                                   unsigned short);
*/


/******************************/
/* Other Misc functions       */
/******************************/

ISC_LONG ISC_EXPORT isc_ftof(const ISC_SCHAR*,
                             const unsigned short,
                             ISC_SCHAR*,
                             const unsigned short);

ISC_STATUS ISC_EXPORT isc_print_blr(const ISC_SCHAR*,
                                    ISC_PRINT_CALLBACK,
                                    void*,
                                    short);

void ISC_EXPORT isc_set_debug(int);

void ISC_EXPORT isc_qtoq(const ISC_QUAD*,
                         ISC_QUAD*);

void ISC_EXPORT isc_vtof(const ISC_SCHAR*,
                         ISC_SCHAR*,
                         unsigned short);

void ISC_EXPORT isc_vtov(const ISC_SCHAR*,
                         ISC_SCHAR*,
                         short);

int ISC_EXPORT isc_version(isc_db_handle*,
                           ISC_VERSION_CALLBACK,
                           void*);

ISC_LONG ISC_EXPORT isc_reset_fpe(ISC_USHORT);


/*****************************************/
/* Service manager functions             */
/*****************************************/


#define ADD_SPB_LENGTH(p, length)	{*(p)++ = (length); \
    *(p)++ = (length) >> 8;}

#define ADD_SPB_NUMERIC(p, data)	{*(p)++ = (ISC_SCHAR) (data); \
    *(p)++ = (ISC_SCHAR) ((data) >> 8); \
    *(p)++ = (ISC_SCHAR) ((data) >> 16); \
    *(p)++ = (ISC_SCHAR) ((data) >> 24);}

ISC_STATUS ISC_EXPORT isc_service_attach(ISC_STATUS*,
                                         unsigned short,
                                         const ISC_SCHAR*,
                                         isc_svc_handle*,
                                         unsigned short,
                                         const ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_service_detach(ISC_STATUS *,
                                         isc_svc_handle *);

ISC_STATUS ISC_EXPORT isc_service_query(ISC_STATUS*,
                                        isc_svc_handle*,
                                        isc_resv_handle*,
                                        unsigned short,
                                        const ISC_SCHAR*,
                                        unsigned short,
                                        const ISC_SCHAR*,
                                        unsigned short,
                                        ISC_SCHAR*);

ISC_STATUS ISC_EXPORT isc_service_start(ISC_STATUS*,
                                        isc_svc_handle*,
                                        isc_resv_handle*,
                                        unsigned short,
                                        const ISC_SCHAR*);


/********************************/
/* Client information functions */
/********************************/

void ISC_EXPORT isc_get_client_version ( ISC_SCHAR  *);
int  ISC_EXPORT isc_get_client_major_version ();
int  ISC_EXPORT isc_get_client_minor_version ();

#ifdef __cplusplus
}	/* extern "C" */
#endif


/***************************************************/
/* Actions to pass to the blob filter (ctl_source) */
/***************************************************/

constexpr int isc_blob_filter_open        = 0;
constexpr int isc_blob_filter_get_segment = 1;
constexpr int isc_blob_filter_close       = 2;
constexpr int isc_blob_filter_create      = 3;
constexpr int isc_blob_filter_put_segment = 4;
constexpr int isc_blob_filter_alloc       = 5;
constexpr int isc_blob_filter_free        = 6;
constexpr int isc_blob_filter_seek        = 7;

/*******************/
/* Blr definitions */
/*******************/


#ifndef JRD_BLR_H
#define JRD_BLR_H

constexpr unsigned char blr_text      = 14;
constexpr unsigned char blr_text2     = 15;
constexpr unsigned char blr_short     = 7;
constexpr unsigned char blr_long      = 8;
constexpr unsigned char blr_quad      = 9;
constexpr unsigned char blr_float     = 10;
constexpr unsigned char blr_double    = 27;
constexpr unsigned char blr_d_float   = 11;
constexpr unsigned char blr_timestamp = 35;
constexpr unsigned char blr_varying   = 37;
constexpr unsigned char blr_varying2  = 38;
constexpr unsigned short blr_blob     = 261;
constexpr unsigned char blr_cstring   = 40;
constexpr unsigned char blr_cstring2  = 41;
constexpr unsigned char blr_blob_id   = 45;
constexpr unsigned char blr_sql_date  = 12;
constexpr unsigned char blr_sql_time  = 13;
constexpr unsigned char blr_int64     = 16;
constexpr unsigned char blr_blob2     = 17;

constexpr unsigned char blr_date = blr_timestamp;

constexpr unsigned char blr_inner = 0;
constexpr unsigned char blr_left  = 1;
constexpr unsigned char blr_right = 2;
constexpr unsigned char blr_full  = 3;

constexpr unsigned char blr_gds_code      = 0;
constexpr unsigned char blr_sql_code      = 1;
constexpr unsigned char blr_exception     = 2;
constexpr unsigned char blr_trigger_code  = 3;
constexpr unsigned char blr_default_code  = 4;
constexpr unsigned char blr_raise         = 5;
constexpr unsigned char blr_exception_msg = 6;

constexpr unsigned char blr_version4 = 4;
constexpr unsigned char blr_version5 = 5;
constexpr unsigned char blr_eoc      = 76;
constexpr unsigned char blr_end      = 255;

constexpr unsigned char blr_assignment     = 1;
constexpr unsigned char blr_begin          = 2;
constexpr unsigned char blr_dcl_variable   = 3;
constexpr unsigned char blr_message        = 4;
constexpr unsigned char blr_erase          = 5;
constexpr unsigned char blr_fetch          = 6;
constexpr unsigned char blr_for            = 7;
constexpr unsigned char blr_if             = 8;
constexpr unsigned char blr_loop           = 9;
constexpr unsigned char blr_modify         = 10;
constexpr unsigned char blr_handler        = 11;
constexpr unsigned char blr_receive        = 12;
constexpr unsigned char blr_select         = 13;
constexpr unsigned char blr_send           = 14;
constexpr unsigned char blr_store          = 15;
constexpr unsigned char blr_label          = 17;
constexpr unsigned char blr_leave          = 18;
constexpr unsigned char blr_store2         = 19;
constexpr unsigned char blr_post           = 20;
constexpr unsigned char blr_literal        = 21;
constexpr unsigned char blr_dbkey          = 22;
constexpr unsigned char blr_field          = 23;
constexpr unsigned char blr_fid            = 24;
constexpr unsigned char blr_parameter      = 25;
constexpr unsigned char blr_variable       = 26;
constexpr unsigned char blr_average        = 27;
constexpr unsigned char blr_count          = 28;
constexpr unsigned char blr_maximum        = 29;
constexpr unsigned char blr_minimum        = 30;
constexpr unsigned char blr_total          = 31;

constexpr unsigned char blr_add            = 34;
constexpr unsigned char blr_subtract       = 35;
constexpr unsigned char blr_multiply       = 36;
constexpr unsigned char blr_divide         = 37;
constexpr unsigned char blr_negate         = 38;
constexpr unsigned char blr_concatenate    = 39;
constexpr unsigned char blr_substring      = 40;
constexpr unsigned char blr_parameter2     = 41;
constexpr unsigned char blr_from           = 42;
constexpr unsigned char blr_via            = 43;
constexpr unsigned char blr_parameter2_old = 44;
constexpr unsigned char blr_user_name      = 44;
constexpr unsigned char blr_null           = 45;

constexpr unsigned char blr_equiv          = 46;
constexpr unsigned char blr_eql            = 47;
constexpr unsigned char blr_neq            = 48;
constexpr unsigned char blr_gtr            = 49;
constexpr unsigned char blr_geq            = 50;
constexpr unsigned char blr_lss            = 51;
constexpr unsigned char blr_leq            = 52;
constexpr unsigned char blr_containing     = 53;
constexpr unsigned char blr_matching       = 54;
constexpr unsigned char blr_starting       = 55;
constexpr unsigned char blr_between        = 56;
constexpr unsigned char blr_or             = 57;
constexpr unsigned char blr_and            = 58;
constexpr unsigned char blr_not            = 59;
constexpr unsigned char blr_any            = 60;
constexpr unsigned char blr_missing        = 61;
constexpr unsigned char blr_unique         = 62;
constexpr unsigned char blr_like           = 63;

constexpr unsigned char blr_rse            = 67;
constexpr unsigned char blr_first          = 68;
constexpr unsigned char blr_project        = 69;
constexpr unsigned char blr_sort           = 70;
constexpr unsigned char blr_boolean        = 71;
constexpr unsigned char blr_ascending      = 72;
constexpr unsigned char blr_descending     = 73;
constexpr unsigned char blr_relation       = 74;
constexpr unsigned char blr_rid            = 75;
constexpr unsigned char blr_union          = 76;
constexpr unsigned char blr_map            = 77;
constexpr unsigned char blr_group_by       = 78;
constexpr unsigned char blr_aggregate      = 79;
constexpr unsigned char blr_join_type      = 80;

constexpr unsigned char blr_agg_count            = 83;
constexpr unsigned char blr_agg_max              = 84;
constexpr unsigned char blr_agg_min              = 85;
constexpr unsigned char blr_agg_total            = 86;
constexpr unsigned char blr_agg_average          = 87;
constexpr unsigned char blr_parameter3           = 88;
constexpr unsigned char blr_run_max              = 89;
constexpr unsigned char blr_run_min              = 90;
constexpr unsigned char blr_run_total            = 91;
constexpr unsigned char blr_run_average          = 92;
constexpr unsigned char blr_agg_count2           = 93;
constexpr unsigned char blr_agg_count_distinct   = 94;
constexpr unsigned char blr_agg_total_distinct   = 95;
constexpr unsigned char blr_agg_average_distinct = 96;

constexpr unsigned char blr_function   = 100;
constexpr unsigned char blr_gen_id     = 101;
constexpr unsigned char blr_prot_mask  = 102;
constexpr unsigned char blr_upcase     = 103;
constexpr unsigned char blr_lock_state = 104;
constexpr unsigned char blr_value_if   = 105;
constexpr unsigned char blr_matching2  = 106;
constexpr unsigned char blr_index      = 107;
constexpr unsigned char blr_ansi_like  = 108;

constexpr unsigned char blr_seek = 112;

constexpr unsigned char blr_continue     = 0;
constexpr unsigned char blr_forward      = 1;
constexpr unsigned char blr_backward     = 2;
constexpr unsigned char blr_bof_forward  = 3;
constexpr unsigned char blr_eof_backward = 4;

constexpr unsigned char blr_run_count = 118;
constexpr unsigned char blr_rs_stream = 119;
constexpr unsigned char blr_exec_proc = 120;

constexpr unsigned char blr_procedure       = 124;
constexpr unsigned char blr_pid             = 125;
constexpr unsigned char blr_exec_pid        = 126;
constexpr unsigned char blr_singular        = 127;
constexpr unsigned char blr_abort           = 128;
constexpr unsigned char blr_block           = 129;
constexpr unsigned char blr_error_handler   = 130;

constexpr unsigned char blr_cast            = 131;

constexpr unsigned char blr_start_savepoint = 134;
constexpr unsigned char blr_end_savepoint   = 135;

constexpr unsigned char blr_plan            = 139;
constexpr unsigned char blr_merge           = 140;
constexpr unsigned char blr_join            = 141;
constexpr unsigned char blr_sequential      = 142;
constexpr unsigned char blr_navigational    = 143;
constexpr unsigned char blr_indices         = 144;
constexpr unsigned char blr_retrieve        = 145;

constexpr unsigned char blr_relation2       = 146;
constexpr unsigned char blr_rid2            = 147;

constexpr unsigned char blr_set_generator   = 150;

constexpr unsigned char blr_ansi_any        = 151;
constexpr unsigned char blr_exists          = 152;

constexpr unsigned char blr_record_version  = 154;
constexpr unsigned char blr_stall           = 155;

constexpr unsigned char blr_ansi_all        = 158;

constexpr unsigned char blr_extract         = 159;

constexpr unsigned char blr_extract_year      = 0;
constexpr unsigned char blr_extract_month     = 1;
constexpr unsigned char blr_extract_day       = 2;
constexpr unsigned char blr_extract_hour      = 3;
constexpr unsigned char blr_extract_minute    = 4;
constexpr unsigned char blr_extract_second    = 5;
constexpr unsigned char blr_extract_weekday   = 6;
constexpr unsigned char blr_extract_yearday   = 7;

constexpr unsigned char blr_current_date      = 160;
constexpr unsigned char blr_current_timestamp = 161;
constexpr unsigned char blr_current_time      = 162;

constexpr unsigned char blr_current_role      = 174;
constexpr unsigned char blr_skip              = 175;

constexpr unsigned char blr_exec_sql          = 176;
constexpr unsigned char blr_internal_info     = 177;
constexpr unsigned char blr_nullsfirst        = 178;
constexpr unsigned char blr_writelock         = 179;
constexpr unsigned char blr_nullslast         = 180;

constexpr unsigned char blr_lowcase           = 181;
constexpr unsigned char blr_strlen            = 182;

constexpr unsigned char blr_strlen_bit        = 0;
constexpr unsigned char blr_strlen_char       = 1;
constexpr unsigned char blr_strlen_octet      = 2;

constexpr unsigned char blr_trim                     = 183;

constexpr unsigned char blr_trim_both                = 0;
constexpr unsigned char blr_trim_leading             = 1;
constexpr unsigned char blr_trim_trailing            = 2;

constexpr unsigned char blr_trim_spaces              = 0;
constexpr unsigned char blr_trim_characters          = 1;

constexpr unsigned char blr_post_arg                 = 163;
constexpr unsigned char blr_exec_into                = 164;
constexpr unsigned char blr_user_savepoint           = 165;
constexpr unsigned char blr_dcl_cursor               = 166;
constexpr unsigned char blr_cursor_stmt              = 167;
constexpr unsigned char blr_current_timestamp2       = 168;
constexpr unsigned char blr_current_time2            = 169;

constexpr unsigned char blr_savepoint_set            = 0;
constexpr unsigned char blr_savepoint_release        = 1;
constexpr unsigned char blr_savepoint_undo           = 2;
constexpr unsigned char blr_savepoint_release_single = 3;

constexpr unsigned char blr_cursor_open              = 0;
constexpr unsigned char blr_cursor_close             = 1;
constexpr unsigned char blr_cursor_fetch             = 2;

#endif 


/**********************************/
/* Database parameter block stuff */
/**********************************/

constexpr int isc_dpb_version1                 = 1;
constexpr int isc_dpb_cdd_pathname             = 1;
constexpr int isc_dpb_allocation               = 2;
constexpr int isc_dpb_journal                  = 3;
constexpr int isc_dpb_page_size                = 4;
constexpr int isc_dpb_num_buffers              = 5;
constexpr int isc_dpb_buffer_length            = 6;
constexpr int isc_dpb_debug                    = 7;
constexpr int isc_dpb_garbage_collect          = 8;
constexpr int isc_dpb_verify                   = 9;
constexpr int isc_dpb_sweep                    = 10;
constexpr int isc_dpb_enable_journal           = 11;
constexpr int isc_dpb_disable_journal          = 12;
constexpr int isc_dpb_dbkey_scope              = 13;
constexpr int isc_dpb_number_of_users          = 14;
constexpr int isc_dpb_trace                    = 15;
constexpr int isc_dpb_no_garbage_collect       = 16;
constexpr int isc_dpb_damaged                  = 17;
constexpr int isc_dpb_license                  = 18;
constexpr int isc_dpb_sys_user_name            = 19;
constexpr int isc_dpb_encrypt_key              = 20;
constexpr int isc_dpb_activate_shadow          = 21;
constexpr int isc_dpb_sweep_interval           = 22;
constexpr int isc_dpb_delete_shadow            = 23;
constexpr int isc_dpb_force_write              = 24;
constexpr int isc_dpb_begin_log                = 25;
constexpr int isc_dpb_quit_log                 = 26;
constexpr int isc_dpb_no_reserve               = 27;
constexpr int isc_dpb_user_name                = 28;
constexpr int isc_dpb_password                 = 29;
constexpr int isc_dpb_password_enc             = 30;
constexpr int isc_dpb_sys_user_name_enc        = 31;
constexpr int isc_dpb_interp                   = 32;
constexpr int isc_dpb_online_dump              = 33;
constexpr int isc_dpb_old_file_size            = 34;
constexpr int isc_dpb_old_num_files            = 35;
constexpr int isc_dpb_old_file                 = 36;
constexpr int isc_dpb_old_start_page           = 37;
constexpr int isc_dpb_old_start_seqno          = 38;
constexpr int isc_dpb_old_start_file           = 39;
constexpr int isc_dpb_drop_walfile             = 40;
constexpr int isc_dpb_old_dump_id              = 41;
constexpr int isc_dpb_wal_backup_dir           = 42;
constexpr int isc_dpb_wal_chkptlen             = 43;
constexpr int isc_dpb_wal_numbufs              = 44;
constexpr int isc_dpb_wal_bufsize              = 45;
constexpr int isc_dpb_wal_grp_cmt_wait         = 46;
constexpr int isc_dpb_lc_messages              = 47;
constexpr int isc_dpb_lc_ctype                 = 48;
constexpr int isc_dpb_cache_manager            = 49;
constexpr int isc_dpb_shutdown                 = 50;
constexpr int isc_dpb_online                   = 51;
constexpr int isc_dpb_shutdown_delay           = 52;
constexpr int isc_dpb_reserved                 = 53;
constexpr int isc_dpb_overwrite                = 54;
constexpr int isc_dpb_sec_attach               = 55;
constexpr int isc_dpb_disable_wal              = 56;
constexpr int isc_dpb_connect_timeout          = 57;
constexpr int isc_dpb_dummy_packet_interval    = 58;
constexpr int isc_dpb_gbak_attach              = 59;
constexpr int isc_dpb_sql_role_name            = 60;
constexpr int isc_dpb_set_page_buffers         = 61;
constexpr int isc_dpb_working_directory        = 62;
constexpr int isc_dpb_sql_dialect              = 63;
constexpr int isc_dpb_set_db_readonly          = 64;
constexpr int isc_dpb_set_db_sql_dialect       = 65;
constexpr int isc_dpb_gfix_attach              = 66;
constexpr int isc_dpb_gstat_attach             = 67;
constexpr int isc_dpb_set_db_charset           = 68;
constexpr int isc_dpb_gsec_attach              = 69;
constexpr int isc_dpb_address_path             = 70;

/**************************************************/
/* clumplet tags used inside isc_dpb_address_path */
/*						 and isc_spb_address_path */
/**************************************************/

/* Format of this clumplet is the following:

 <address-path-clumplet> ::=
    isc_dpb_address_path <byte-clumplet-length> <address-stack>

 <address-stack> ::=
    <address-descriptor> |
    <address-stack> <address-descriptor>

 <address-descriptor> ::=
    isc_dpb_address <byte-clumplet-length> <address-elements>

 <address-elements> ::=
    <address-element> |
    <address-elements> <address-element>

 <address-element> ::=
    isc_dpb_addr_protocol <byte-clumplet-length> <protocol-string> |
    isc_dpb_addr_endpoint <byte-clumplet-length> <remote-endpoint-string>

 <protocol-string> ::=
    "TCPv4" |
    "TCPv6" |
    "XNET" |
    "WNET" |
    ....

 <remote-endpoint-string> ::=
    <IPv4-address> | // such as "172.20.1.1"
    <IPv6-address> | // such as "2001:0:13FF:09FF::1"
    <xnet-process-id> | // such as "17864"
    ...
*/

constexpr int isc_dpb_address       = 1;

constexpr int isc_dpb_addr_protocol = 1;
constexpr int isc_dpb_addr_endpoint = 2;

/*********************************/
/* isc_dpb_verify specific flags */
/*********************************/

constexpr int isc_dpb_pages        = 1;
constexpr int isc_dpb_records      = 2;
constexpr int isc_dpb_indices      = 4;
constexpr int isc_dpb_transactions = 8;
constexpr int isc_dpb_no_update    = 16;
constexpr int isc_dpb_repair       = 32;
constexpr int isc_dpb_ignore       = 64;

/***********************************/
/* isc_dpb_shutdown specific flags */
/***********************************/

constexpr int isc_dpb_shut_cache       = 0x01;
constexpr int isc_dpb_shut_attachment  = 0x02;
constexpr int isc_dpb_shut_transaction = 0x04;
constexpr int isc_dpb_shut_force       = 0x08;
constexpr int isc_dpb_shut_mode_mask   = 0x70;
constexpr int isc_dpb_shut_default     = 0x00;
constexpr int isc_dpb_shut_normal      = 0x10;
constexpr int isc_dpb_shut_multi       = 0x20;
constexpr int isc_dpb_shut_single      = 0x30;
constexpr int isc_dpb_shut_full        = 0x40;

/**************************************/
/* Bit assignments in RDB$SYSTEM_FLAG */
/**************************************/

constexpr int RDB_system      = 1;
constexpr int RDB_id_assigned = 2;
/* 2 is for QLI. See jrd/constants.h for more Firebird-specific values. */


/*************************************/
/* Transaction parameter block stuff */
/*************************************/

constexpr int isc_tpb_version1         = 1;
constexpr int isc_tpb_version3         = 3;
constexpr int isc_tpb_consistency      = 1;
constexpr int isc_tpb_concurrency      = 2;
constexpr int isc_tpb_shared           = 3;
constexpr int isc_tpb_protected        = 4;
constexpr int isc_tpb_exclusive        = 5;
constexpr int isc_tpb_wait             = 6;
constexpr int isc_tpb_nowait           = 7;
constexpr int isc_tpb_read             = 8;
constexpr int isc_tpb_write            = 9;
constexpr int isc_tpb_lock_read        = 10;
constexpr int isc_tpb_lock_write       = 11;
constexpr int isc_tpb_verb_time        = 12;
constexpr int isc_tpb_commit_time      = 13;
constexpr int isc_tpb_ignore_limbo     = 14;
constexpr int isc_tpb_read_committed   = 15;
constexpr int isc_tpb_autocommit       = 16;
constexpr int isc_tpb_rec_version      = 17;
constexpr int isc_tpb_no_rec_version   = 18;
constexpr int isc_tpb_restart_requests = 19;
constexpr int isc_tpb_no_auto_undo     = 20;
constexpr int isc_tpb_lock_timeout     = 21;


/************************/
/* Blob Parameter Block */
/************************/

constexpr int isc_bpb_version1         = 1;
constexpr int isc_bpb_source_type      = 1;
constexpr int isc_bpb_target_type      = 2;
constexpr int isc_bpb_type             = 3;
constexpr int isc_bpb_source_interp    = 4;
constexpr int isc_bpb_target_interp    = 5;
constexpr int isc_bpb_filter_parameter = 6;
constexpr int isc_bpb_type_segmented   = 0;
constexpr int isc_bpb_type_stream      = 1;


/*********************************/
/* Service parameter block stuff */
/*********************************/

constexpr int isc_spb_version1          = 1;
constexpr int isc_spb_current_version   = 2;
constexpr int isc_spb_version		    = isc_spb_current_version;
constexpr int isc_spb_user_name         = isc_dpb_user_name;
constexpr int isc_spb_sys_user_name     = isc_dpb_sys_user_name;
constexpr int isc_spb_sys_user_name_enc = isc_dpb_sys_user_name_enc;
constexpr int isc_spb_password          = isc_dpb_password;
constexpr int isc_spb_password_enc      = isc_dpb_password_enc;
constexpr int isc_spb_command_line      = 105;
constexpr int isc_spb_dbname            = 106;
constexpr int isc_spb_verbose           = 107;
constexpr int isc_spb_options           = 108;
constexpr int isc_spb_address_path      = 109;


constexpr int isc_spb_connect_timeout       = isc_dpb_connect_timeout;
constexpr int isc_spb_dummy_packet_interval = isc_dpb_dummy_packet_interval;
constexpr int isc_spb_sql_role_name         = isc_dpb_sql_role_name;


/*********************************/
/* Information call declarations */
/*********************************/


#ifndef JRD_INF_PUB_H
#define JRD_INF_PUB_H

constexpr int isc_info_end            = 1;
constexpr int isc_info_truncated      = 2;
constexpr int isc_info_error          = 3;
constexpr int isc_info_data_not_ready = 4;
constexpr int isc_info_flag_end       = 127;

enum db_info_types
{
    isc_info_db_id                  = 4,
    isc_info_reads                  = 5,
    isc_info_writes                 = 6,
    isc_info_fetches                = 7,
    isc_info_marks                  = 8,

    isc_info_implementation         = 11,
    isc_info_isc_version            = 12,
    isc_info_base_level             = 13,
    isc_info_page_size              = 14,
    isc_info_num_buffers            = 15,
    isc_info_limbo                  = 16,
    isc_info_current_memory         = 17,
    isc_info_max_memory             = 18,
    isc_info_window_turns           = 19,
    isc_info_license                = 20,

    isc_info_allocation             = 21,
    isc_info_attachment_id          = 22,
    isc_info_read_seq_count         = 23,
    isc_info_read_idx_count         = 24,
    isc_info_insert_count           = 25,
    isc_info_update_count           = 26,
    isc_info_delete_count           = 27,
    isc_info_backout_count          = 28,
    isc_info_purge_count            = 29,
    isc_info_expunge_count          = 30,

    isc_info_sweep_interval         = 31,
    isc_info_ods_version            = 32,
    isc_info_ods_minor_version      = 33,
    isc_info_no_reserve             = 34,

    isc_info_logfile                = 35,
    isc_info_cur_logfile_name       = 36,
    isc_info_cur_log_part_offset    = 37,
    isc_info_num_wal_buffers        = 38,
    isc_info_wal_buffer_size        = 39,
    isc_info_wal_ckpt_length        = 40,

    isc_info_wal_cur_ckpt_interval  = 41,
    isc_info_wal_prv_ckpt_fname     = 42,
    isc_info_wal_prv_ckpt_poffset   = 43,
    isc_info_wal_recv_ckpt_fname    = 44,
    isc_info_wal_recv_ckpt_poffset  = 45,
    isc_info_wal_grpc_wait_usecs    = 47,
    isc_info_wal_num_io             = 48,
    isc_info_wal_avg_io_size        = 49,
    isc_info_wal_num_commits        = 50,
    isc_info_wal_avg_grpc_size      = 51,

    isc_info_forced_writes          = 52,
    isc_info_user_names             = 53,
    isc_info_page_errors            = 54,
    isc_info_record_errors          = 55,
    isc_info_bpage_errors           = 56,
    isc_info_dpage_errors           = 57,
    isc_info_ipage_errors           = 58,
    isc_info_ppage_errors           = 59,
    isc_info_tpage_errors           = 60,

    isc_info_set_page_buffers       = 61,
    isc_info_db_sql_dialect         = 62,
    isc_info_db_read_only           = 63,
    isc_info_db_size_in_pages       = 64,

    frb_info_att_charset            = 101,
    isc_info_db_class               = 102,
    isc_info_firebird_version       = 103,
    isc_info_oldest_transaction     = 104,
    isc_info_oldest_active          = 105,
    isc_info_oldest_snapshot        = 106,
    isc_info_next_transaction       = 107,
    isc_info_db_provider            = 108,
    isc_info_active_transactions    = 109,
    isc_info_active_tran_count      = 110,
    isc_info_creation_date          = 111,

    isc_info_db_last_value
};

constexpr int isc_info_version = isc_info_isc_version;

enum  info_db_implementations
{
    isc_info_db_impl_rdb_vms = 1,
    isc_info_db_impl_rdb_eln = 2,
    isc_info_db_impl_rdb_eln_dev = 3,
    isc_info_db_impl_rdb_vms_y = 4,
    isc_info_db_impl_rdb_eln_y = 5,
    isc_info_db_impl_jri = 6,
    isc_info_db_impl_jsv = 7,

    isc_info_db_impl_isc_apl_68K = 25,
    isc_info_db_impl_isc_vax_ultr = 26,
    isc_info_db_impl_isc_vms = 27,
    isc_info_db_impl_isc_sun_68k = 28,
    isc_info_db_impl_isc_os2 = 29,
    isc_info_db_impl_isc_sun4 = 30,

    isc_info_db_impl_isc_hp_ux = 31,
    isc_info_db_impl_isc_sun_386i = 32,
    isc_info_db_impl_isc_vms_orcl = 33,
    isc_info_db_impl_isc_mac_aux = 34,
    isc_info_db_impl_isc_rt_aix = 35,
    isc_info_db_impl_isc_mips_ult = 36,
    isc_info_db_impl_isc_xenix = 37,
    isc_info_db_impl_isc_dg = 38,
    isc_info_db_impl_isc_hp_mpexl = 39,
    isc_info_db_impl_isc_hp_ux68K = 40,

    isc_info_db_impl_isc_sgi = 41,
    isc_info_db_impl_isc_sco_unix = 42,
    isc_info_db_impl_isc_cray = 43,
    isc_info_db_impl_isc_imp = 44,
    isc_info_db_impl_isc_delta = 45,
    isc_info_db_impl_isc_next = 46,
    isc_info_db_impl_isc_dos = 47,
    isc_info_db_impl_m88K = 48,
    isc_info_db_impl_unixware = 49,
    isc_info_db_impl_isc_winnt_x86 = 50,

    isc_info_db_impl_isc_epson = 51,
    isc_info_db_impl_alpha_osf = 52,
    isc_info_db_impl_alpha_vms = 53,
    isc_info_db_impl_netware_386 = 54,
    isc_info_db_impl_win_only = 55,
    isc_info_db_impl_ncr_3000 = 56,
    isc_info_db_impl_winnt_ppc = 57,
    isc_info_db_impl_dg_x86 = 58,
    isc_info_db_impl_sco_ev = 59,
    isc_info_db_impl_i386 = 60,

    isc_info_db_impl_freebsd = 61,
    isc_info_db_impl_netbsd = 62,
    isc_info_db_impl_darwin = 63,
    isc_info_db_impl_sinixz = 64,

    isc_info_db_impl_linux_sparc = 65,
    isc_info_db_impl_linux_amd64 = 66,

    isc_info_db_impl_freebsd_amd64 = 67,

    isc_info_db_impl_last_value
};

constexpr int isc_info_db_impl_isc_a = isc_info_db_impl_isc_apl_68K;
constexpr int isc_info_db_impl_isc_u = isc_info_db_impl_isc_vax_ultr;
constexpr int isc_info_db_impl_isc_v = isc_info_db_impl_isc_vms;
constexpr int isc_info_db_impl_isc_s = isc_info_db_impl_isc_sun_68k;

enum info_db_class
{
    isc_info_db_class_access = 1,
    isc_info_db_class_y_valve = 2,
    isc_info_db_class_rem_int = 3,
    isc_info_db_class_rem_srvr = 4,
    isc_info_db_class_pipe_int = 7,
    isc_info_db_class_pipe_srvr = 8,
    isc_info_db_class_sam_int = 9,
    isc_info_db_class_sam_srvr = 10,
    isc_info_db_class_gateway = 11,
    isc_info_db_class_cache = 12,
    isc_info_db_class_classic_access = 13,
    isc_info_db_class_server_access = 14,

    isc_info_db_class_last_value
};

enum info_db_provider
{
    isc_info_db_code_rdb_eln = 1,
    isc_info_db_code_rdb_vms = 2,
    isc_info_db_code_interbase = 3,
    isc_info_db_code_firebird = 4,

    isc_info_db_code_last_value
};

constexpr int isc_info_number_messages        = 4;
constexpr int isc_info_max_message            = 5;
constexpr int isc_info_max_send               = 6;
constexpr int isc_info_max_receive            = 7;
constexpr int isc_info_state                  = 8;
constexpr int isc_info_message_number         = 9;
constexpr int isc_info_message_size           = 10 ;
constexpr int isc_info_request_cost           = 11 ;
constexpr int isc_info_access_path            = 12 ;
constexpr int isc_info_req_select_count       = 13 ;
constexpr int isc_info_req_insert_count       = 14 ;
constexpr int isc_info_req_update_count       = 15 ;
constexpr int isc_info_req_delete_count       = 16 ;

constexpr int isc_info_rsb_end                = 0;
constexpr int isc_info_rsb_begin              = 1;
constexpr int isc_info_rsb_type               = 2;
constexpr int isc_info_rsb_relation           = 3;
constexpr int isc_info_rsb_plan               = 4;

constexpr int isc_info_rsb_unknown            = 1;
constexpr int isc_info_rsb_indexed            = 2;
constexpr int isc_info_rsb_navigate           = 3;
constexpr int isc_info_rsb_sequential         = 4;
constexpr int isc_info_rsb_cross              = 5;
constexpr int isc_info_rsb_sort               = 6;
constexpr int isc_info_rsb_first              = 7;
constexpr int isc_info_rsb_boolean            = 8;
constexpr int isc_info_rsb_union              = 9;
constexpr int isc_info_rsb_aggregate          = 10;
constexpr int isc_info_rsb_merge              = 11;
constexpr int isc_info_rsb_ext_sequential     = 12;
constexpr int isc_info_rsb_ext_indexed        = 13;
constexpr int isc_info_rsb_ext_dbkey          = 14;
constexpr int isc_info_rsb_left_cross         = 15;
constexpr int isc_info_rsb_select             = 16;
constexpr int isc_info_rsb_sql_join           = 17;
constexpr int isc_info_rsb_simulate           = 18;
constexpr int isc_info_rsb_sim_cross          = 19;
constexpr int isc_info_rsb_once               = 20;
constexpr int isc_info_rsb_procedure          = 21;
constexpr int isc_info_rsb_skip               = 22;

constexpr int isc_info_rsb_and                = 1;
constexpr int isc_info_rsb_or                 = 2;
constexpr int isc_info_rsb_dbkey              = 3;
constexpr int isc_info_rsb_index              = 4;

constexpr int isc_info_req_active             = 2;
constexpr int isc_info_req_inactive           = 3;
constexpr int isc_info_req_send               = 4;
constexpr int isc_info_req_receive            = 5;
constexpr int isc_info_req_select             = 6;
constexpr int isc_info_req_sql_stall          = 7;

constexpr int isc_info_blob_num_segments      = 4;
constexpr int isc_info_blob_max_segment       = 5;
constexpr int isc_info_blob_total_length      = 6;
constexpr int isc_info_blob_type              = 7;

constexpr int isc_info_tra_id                  = 4;
constexpr int isc_info_tra_oldest_interesting  = 5;
constexpr int isc_info_tra_oldest_snapshot     = 6;
constexpr int isc_info_tra_oldest_active       = 7;
constexpr int isc_info_tra_isolation           = 8;
constexpr int isc_info_tra_access              = 9;
constexpr int isc_info_tra_lock_timeout        = 10;

constexpr int isc_info_tra_consistency         = 1;
constexpr int isc_info_tra_concurrency         = 2;
constexpr int isc_info_tra_read_committed      = 3;

constexpr int isc_info_tra_no_rec_version      = 0;
constexpr int isc_info_tra_rec_version         = 1;

constexpr int isc_info_tra_readonly       = 0;
constexpr int isc_info_tra_readwrite      = 1;

constexpr int isc_info_sql_select         = 4;
constexpr int isc_info_sql_bind           = 5;
constexpr int isc_info_sql_num_variables  = 6;
constexpr int isc_info_sql_describe_vars  = 7;
constexpr int isc_info_sql_describe_end   = 8;
constexpr int isc_info_sql_sqlda_seq      = 9;
constexpr int isc_info_sql_message_seq    = 10;
constexpr int isc_info_sql_type           = 11;
constexpr int isc_info_sql_sub_type       = 12;
constexpr int isc_info_sql_scale          = 13;
constexpr int isc_info_sql_length         = 14;
constexpr int isc_info_sql_null_ind       = 15;
constexpr int isc_info_sql_field          = 16;
constexpr int isc_info_sql_relation       = 17;
constexpr int isc_info_sql_owner          = 18;
constexpr int isc_info_sql_alias          = 19;
constexpr int isc_info_sql_sqlda_start    = 20;
constexpr int isc_info_sql_stmt_type      = 21;
constexpr int isc_info_sql_get_plan       = 22;
constexpr int isc_info_sql_records        = 23;
constexpr int isc_info_sql_batch_fetch    = 24;
constexpr int isc_info_sql_relation_alias = 25;

constexpr int isc_info_sql_stmt_select          = 1;
constexpr int isc_info_sql_stmt_insert          = 2;
constexpr int isc_info_sql_stmt_update          = 3;
constexpr int isc_info_sql_stmt_delete          = 4;
constexpr int isc_info_sql_stmt_ddl             = 5;
constexpr int isc_info_sql_stmt_get_segment     = 6;
constexpr int isc_info_sql_stmt_put_segment     = 7;
constexpr int isc_info_sql_stmt_exec_procedure  = 8;
constexpr int isc_info_sql_stmt_start_trans     = 9;
constexpr int isc_info_sql_stmt_commit          = 10;
constexpr int isc_info_sql_stmt_rollback        = 11;
constexpr int isc_info_sql_stmt_select_for_upd  = 12;
constexpr int isc_info_sql_stmt_set_generator   = 13;
constexpr int isc_info_sql_stmt_savepoint       = 14;

#endif 


/*****************************
 * Service action items      *
 *****************************/

constexpr int isc_action_svc_backup         = 1;	/* Starts database backup process on the server */
constexpr int isc_action_svc_restore        = 2;	/* Starts database restore process on the server */
constexpr int isc_action_svc_repair         = 3;	/* Starts database repair process on the server */
constexpr int isc_action_svc_add_user       = 4;	/* Adds a new user to the security database */
constexpr int isc_action_svc_delete_user    = 5;	/* Deletes a user record from the security database */
constexpr int isc_action_svc_modify_user    = 6;	/* Modifies a user record in the security database */
constexpr int isc_action_svc_display_user   = 7;	/* Displays a user record from the security database */
constexpr int isc_action_svc_properties     = 8;	/* Sets database properties */
constexpr int isc_action_svc_add_license    = 9;	/* Adds a license to the license file */
constexpr int isc_action_svc_remove_license = 10;	/* Removes a license from the license file */
constexpr int isc_action_svc_db_stats	    = 11;	/* Retrieves database statistics */
constexpr int isc_action_svc_get_ib_log     = 12;	/* Retrieves the InterBase log file from the server */
constexpr int isc_action_svc_get_fb_log     = 12;	/* Retrieves the Firebird log file from the server */

/*****************************
 * Service information items *
 *****************************/

constexpr int isc_info_svc_svr_db_info        = 50;	/* Retrieves the number of attachments and databases */
constexpr int isc_info_svc_get_license        = 51;	/* Retrieves all license keys and IDs from the license file */
constexpr int isc_info_svc_get_license_mask   = 52;	/* Retrieves a bitmask representing licensed options on the server */
constexpr int isc_info_svc_get_config         = 53;	/* Retrieves the parameters and values for IB_CONFIG */
constexpr int isc_info_svc_version            = 54;	/* Retrieves the version of the services manager */
constexpr int isc_info_svc_server_version     = 55;	/* Retrieves the version of the InterBase server */
constexpr int isc_info_svc_implementation     = 56;	/* Retrieves the implementation of the InterBase server */
constexpr int isc_info_svc_capabilities       = 57;	/* Retrieves a bitmask representing the server's capabilities */
constexpr int isc_info_svc_user_dbpath        = 58;	/* Retrieves the path to the security database in use by the server */
constexpr int isc_info_svc_get_env	          = 59;	/* Retrieves the setting of $INTERBASE */
constexpr int isc_info_svc_get_env_lock       = 60;	/* Retrieves the setting of $INTERBASE_LCK */
constexpr int isc_info_svc_get_env_msg        = 61;	/* Retrieves the setting of $INTERBASE_MSG */
constexpr int isc_info_svc_line               = 62;	/* Retrieves 1 line of service output per call */
constexpr int isc_info_svc_to_eof             = 63;	/* Retrieves as much of the server output as will fit in the supplied buffer */
constexpr int isc_info_svc_timeout            = 64;	/* Sets / signifies a timeout value for reading service information */
constexpr int isc_info_svc_get_licensed_users = 65;	/* Retrieves the number of users licensed for accessing the server */
constexpr int isc_info_svc_limbo_trans	      = 66;	/* Retrieve the limbo transactions */
constexpr int isc_info_svc_running		      = 67;	/* Checks to see if a service is running on an attachment */
constexpr int isc_info_svc_get_users		  = 68;	/* Returns the user information from isc_action_svc_display_users */

/******************************************************
 * Parameters for isc_action_{add|del|mod|disp)_user  *
 ******************************************************/

constexpr int isc_spb_sec_userid     = 5;
constexpr int isc_spb_sec_groupid    = 6;
constexpr int isc_spb_sec_username   = 7;
constexpr int isc_spb_sec_password   = 8;
constexpr int isc_spb_sec_groupname  = 9;
constexpr int isc_spb_sec_firstname  = 10;
constexpr int isc_spb_sec_middlename = 11;
constexpr int isc_spb_sec_lastname   = 12;

/*******************************************************
 * Parameters for isc_action_svc_(add|remove)_license, *
 * isc_info_svc_get_license                            *
 *******************************************************/

constexpr int isc_spb_lic_key  = 5;
constexpr int isc_spb_lic_id   = 6;
constexpr int isc_spb_lic_desc = 7;


/*****************************************
 * Parameters for isc_action_svc_backup  *
 *****************************************/

constexpr int isc_spb_bkp_file               = 5;
constexpr int isc_spb_bkp_factor             = 6;
constexpr int isc_spb_bkp_length             = 7;
constexpr int isc_spb_bkp_ignore_checksums   = 0x01;
constexpr int isc_spb_bkp_ignore_limbo       = 0x02;
constexpr int isc_spb_bkp_metadata_only      = 0x04;
constexpr int isc_spb_bkp_no_garbage_collect = 0x08;
constexpr int isc_spb_bkp_old_descriptions   = 0x10;
constexpr int isc_spb_bkp_non_transportable  = 0x20;
constexpr int isc_spb_bkp_convert            = 0x40;
constexpr int isc_spb_bkp_expand             = 0x80;

/********************************************
 * Parameters for isc_action_svc_properties *
 ********************************************/

constexpr int isc_spb_prp_page_buffers		    = 5;
constexpr int isc_spb_prp_sweep_interval	    = 6;
constexpr int isc_spb_prp_shutdown_db		    = 7;
constexpr int isc_spb_prp_deny_new_attachments  = 9;
constexpr int isc_spb_prp_deny_new_transactions = 10;
constexpr int isc_spb_prp_reserve_space		    = 11;
constexpr int isc_spb_prp_write_mode		    = 12;
constexpr int isc_spb_prp_access_mode		    = 13;
constexpr int isc_spb_prp_set_sql_dialect	    = 14;
constexpr int isc_spb_prp_activate			    = 0x0100;
constexpr int isc_spb_prp_db_online			    = 0x0200;

/********************************************
 * Parameters for isc_spb_prp_reserve_space *
 ********************************************/

constexpr int isc_spb_prp_res_use_full	= 35;
constexpr int isc_spb_prp_res			= 36;

/******************************************
 * Parameters for isc_spb_prp_write_mode  *
 ******************************************/

constexpr int isc_spb_prp_wm_async = 37;
constexpr int isc_spb_prp_wm_sync  = 38;

/******************************************
 * Parameters for isc_spb_prp_access_mode *
 ******************************************/

constexpr int isc_spb_prp_am_readonly	= 39;
constexpr int isc_spb_prp_am_readwrite	= 40;

/*****************************************
 * Parameters for isc_action_svc_repair  *
 *****************************************/

constexpr int isc_spb_rpr_commit_trans		= 15;
constexpr int isc_spb_rpr_rollback_trans	= 34;
constexpr int isc_spb_rpr_recover_two_phase	= 17;
constexpr int isc_spb_tra_id				= 18;
constexpr int isc_spb_single_tra_id			= 19;
constexpr int isc_spb_multi_tra_id			= 20;
constexpr int isc_spb_tra_state				= 21;
constexpr int isc_spb_tra_state_limbo		= 22;
constexpr int isc_spb_tra_state_commit		= 23;
constexpr int isc_spb_tra_state_rollback	= 24;
constexpr int isc_spb_tra_state_unknown		= 25;
constexpr int isc_spb_tra_host_site			= 26;
constexpr int isc_spb_tra_remote_site		= 27;
constexpr int isc_spb_tra_db_path			= 28;
constexpr int isc_spb_tra_advise			= 29;
constexpr int isc_spb_tra_advise_commit		= 30;
constexpr int isc_spb_tra_advise_rollback	= 31;
constexpr int isc_spb_tra_advise_unknown	= 33;

constexpr int isc_spb_rpr_validate_db		= 0x01;
constexpr int isc_spb_rpr_sweep_db			= 0x02;
constexpr int isc_spb_rpr_mend_db			= 0x04;
constexpr int isc_spb_rpr_list_limbo_trans	= 0x08;
constexpr int isc_spb_rpr_check_db			= 0x10;
constexpr int isc_spb_rpr_ignore_checksum	= 0x20;
constexpr int isc_spb_rpr_kill_shadows		= 0x40;
constexpr int isc_spb_rpr_full				= 0x80;

/*****************************************
 * Parameters for isc_action_svc_restore *
 *****************************************/

constexpr int isc_spb_res_buffers		   = 9;
constexpr int isc_spb_res_page_size        = 10;
constexpr int isc_spb_res_length		   = 11;
constexpr int isc_spb_res_access_mode	   = 12;
constexpr int isc_spb_res_deactivate_idx   = 0x0100;
constexpr int isc_spb_res_no_shadow	       = 0x0200;
constexpr int isc_spb_res_no_validity	   = 0x0400;
constexpr int isc_spb_res_one_at_a_time    = 0x0800;
constexpr int isc_spb_res_replace		   = 0x1000;
constexpr int isc_spb_res_create		   = 0x2000;
constexpr int isc_spb_res_use_all_space    = 0x4000;

/******************************************
 * Parameters for isc_spb_res_access_mode  *
 ******************************************/

constexpr int isc_spb_res_am_readonly  = isc_spb_prp_am_readonly;
constexpr int isc_spb_res_am_readwrite = isc_spb_prp_am_readwrite;

/*******************************************
 * Parameters for isc_info_svc_svr_db_info *
 *******************************************/

constexpr int isc_spb_num_att = 5;
constexpr int isc_spb_num_db  = 6;

/*****************************************
 * Parameters for isc_info_svc_db_stats  *
 *****************************************/

constexpr int isc_spb_sts_data_pages		= 0x01;
constexpr int isc_spb_sts_db_log			= 0x02;
constexpr int isc_spb_sts_hdr_pages		    = 0x04;
constexpr int isc_spb_sts_idx_pages		    = 0x08;
constexpr int isc_spb_sts_sys_relations	    = 0x10;
constexpr int isc_spb_sts_record_versions	= 0x20;
constexpr int isc_spb_sts_table			    = 0x40;
constexpr int isc_spb_sts_nocreation		= 0x80;

/***********************************/
/* Server configuration key values */
/***********************************/

/* Not available in Firebird 1.5 */


/**********************************************/
/* Dynamic Data Definition Language operators */
/**********************************************/

/******************/
/* Version number */
/******************/

constexpr int isc_dyn_version_1 = 1;
constexpr int isc_dyn_eoc       = 255;

/******************************/
/* Operations (may be nested) */
/******************************/

constexpr int isc_dyn_begin                 = 2;
constexpr int isc_dyn_end                   = 3;
constexpr int isc_dyn_if                    = 4;
constexpr int isc_dyn_def_database          = 5;
constexpr int isc_dyn_def_global_fld        = 6;
constexpr int isc_dyn_def_local_fld         = 7;
constexpr int isc_dyn_def_idx               = 8;
constexpr int isc_dyn_def_rel               = 9;
constexpr int isc_dyn_def_sql_fld           = 10;
constexpr int isc_dyn_def_view              = 12;
constexpr int isc_dyn_def_trigger           = 15;
constexpr int isc_dyn_def_security_class    = 120;
constexpr int isc_dyn_def_dimension         = 140;
constexpr int isc_dyn_def_generator         = 24;
constexpr int isc_dyn_def_function          = 25;
constexpr int isc_dyn_def_filter            = 26;
constexpr int isc_dyn_def_function_arg      = 27;
constexpr int isc_dyn_def_shadow            = 34;
constexpr int isc_dyn_def_trigger_msg       = 17;
constexpr int isc_dyn_def_file              = 36;
constexpr int isc_dyn_mod_database          = 39;
constexpr int isc_dyn_mod_rel               = 11;
constexpr int isc_dyn_mod_global_fld        = 13;
constexpr int isc_dyn_mod_idx               = 102;
constexpr int isc_dyn_mod_local_fld         = 14;
constexpr int isc_dyn_mod_sql_fld		    = 216;
constexpr int isc_dyn_mod_view              = 16;
constexpr int isc_dyn_mod_security_class    = 122;
constexpr int isc_dyn_mod_trigger           = 113;
constexpr int isc_dyn_mod_trigger_msg       = 28;
constexpr int isc_dyn_delete_database       = 18;
constexpr int isc_dyn_delete_rel            = 19;
constexpr int isc_dyn_delete_global_fld     = 20;
constexpr int isc_dyn_delete_local_fld      = 21;
constexpr int isc_dyn_delete_idx            = 22;
constexpr int isc_dyn_delete_security_class = 123;
constexpr int isc_dyn_delete_dimensions     = 143;
constexpr int isc_dyn_delete_trigger        = 23;
constexpr int isc_dyn_delete_trigger_msg    = 29;
constexpr int isc_dyn_delete_filter         = 32;
constexpr int isc_dyn_delete_function       = 33;
constexpr int isc_dyn_delete_shadow         = 35;
constexpr int isc_dyn_grant                 = 30;
constexpr int isc_dyn_revoke                = 31;
constexpr int isc_dyn_def_primary_key       = 37;
constexpr int isc_dyn_def_foreign_key       = 38;
constexpr int isc_dyn_def_unique            = 40;
constexpr int isc_dyn_def_procedure         = 164;
constexpr int isc_dyn_delete_procedure      = 165;
constexpr int isc_dyn_def_parameter         = 135;
constexpr int isc_dyn_delete_parameter      = 136;
constexpr int isc_dyn_mod_procedure         = 175;
/* Deprecated.
#define isc_dyn_def_log_file              176
#define isc_dyn_def_cache_file            180
*/
constexpr int isc_dyn_def_exception = 181;
constexpr int isc_dyn_mod_exception = 182;
constexpr int isc_dyn_del_exception = 183;
/* Deprecated.
#define isc_dyn_drop_log                  194
#define isc_dyn_drop_cache                195
#define isc_dyn_def_default_log           202
*/
constexpr int isc_dyn_def_difference  = 220;
constexpr int isc_dyn_drop_difference = 221;
constexpr int isc_dyn_begin_backup    = 222;
constexpr int isc_dyn_end_backup      = 223;

/***********************/
/* View specific stuff */
/***********************/

constexpr int isc_dyn_view_blr          = 43;
constexpr int isc_dyn_view_source       = 44;
constexpr int isc_dyn_view_relation     = 45;
constexpr int isc_dyn_view_context      = 46;
constexpr int isc_dyn_view_context_name = 47;

/**********************/
/* Generic attributes */
/**********************/

constexpr int isc_dyn_rel_name               = 50;
constexpr int isc_dyn_fld_name               = 51;
constexpr int isc_dyn_new_fld_name		     = 215;
constexpr int isc_dyn_idx_name               = 52;
constexpr int isc_dyn_description            = 53;
constexpr int isc_dyn_security_class         = 54;
constexpr int isc_dyn_system_flag            = 55;
constexpr int isc_dyn_update_flag            = 56;
constexpr int isc_dyn_prc_name               = 166;
constexpr int isc_dyn_prm_name               = 137;
constexpr int isc_dyn_sql_object             = 196;
constexpr int isc_dyn_fld_character_set_name = 174;

/********************************/
/* Relation specific attributes */
/********************************/

constexpr int isc_dyn_rel_dbkey_length       = 61;
constexpr int isc_dyn_rel_store_trig         = 62;
constexpr int isc_dyn_rel_modify_trig        = 63;
constexpr int isc_dyn_rel_erase_trig         = 64;
constexpr int isc_dyn_rel_store_trig_source  = 65;
constexpr int isc_dyn_rel_modify_trig_source = 66;
constexpr int isc_dyn_rel_erase_trig_source  = 67;
constexpr int isc_dyn_rel_ext_file           = 68;
constexpr int isc_dyn_rel_sql_protection     = 69;
constexpr int isc_dyn_rel_constraint         = 162;
constexpr int isc_dyn_delete_rel_constraint  = 163;

/************************************/
/* Global field specific attributes */
/************************************/

constexpr int isc_dyn_fld_type              = 70;
constexpr int isc_dyn_fld_length            = 71;
constexpr int isc_dyn_fld_scale             = 72;
constexpr int isc_dyn_fld_sub_type          = 73;
constexpr int isc_dyn_fld_segment_length    = 74;
constexpr int isc_dyn_fld_query_header      = 75;
constexpr int isc_dyn_fld_edit_string       = 76;
constexpr int isc_dyn_fld_validation_blr    = 77;
constexpr int isc_dyn_fld_validation_source = 78;
constexpr int isc_dyn_fld_computed_blr      = 79;
constexpr int isc_dyn_fld_computed_source   = 80;
constexpr int isc_dyn_fld_missing_value     = 81;
constexpr int isc_dyn_fld_default_value     = 82;
constexpr int isc_dyn_fld_query_name        = 83;
constexpr int isc_dyn_fld_dimensions        = 84;
constexpr int isc_dyn_fld_not_null          = 85;
constexpr int isc_dyn_fld_precision         = 86;
constexpr int isc_dyn_fld_char_length       = 172;
constexpr int isc_dyn_fld_collation         = 173;
constexpr int isc_dyn_fld_default_source    = 193;
constexpr int isc_dyn_del_default           = 197;
constexpr int isc_dyn_del_validation        = 198;
constexpr int isc_dyn_single_validation     = 199;
constexpr int isc_dyn_fld_character_set     = 203;

/***********************************/
/* Local field specific attributes */
/***********************************/

constexpr int isc_dyn_fld_source      = 90;
constexpr int isc_dyn_fld_base_fld    = 91;
constexpr int isc_dyn_fld_position    = 92;
constexpr int isc_dyn_fld_update_flag = 93;

/*****************************/
/* Index specific attributes */
/*****************************/

constexpr int isc_dyn_idx_unique      = 100;
constexpr int isc_dyn_idx_inactive    = 101;
constexpr int isc_dyn_idx_type        = 103;
constexpr int isc_dyn_idx_foreign_key = 104;
constexpr int isc_dyn_idx_ref_column  = 105;
constexpr int isc_dyn_idx_statistic	  = 204;

/*******************************/
/* Trigger specific attributes */
/*******************************/

constexpr int isc_dyn_trg_type       = 110;
constexpr int isc_dyn_trg_blr        = 111;
constexpr int isc_dyn_trg_source     = 112;
constexpr int isc_dyn_trg_name       = 114;
constexpr int isc_dyn_trg_sequence   = 115;
constexpr int isc_dyn_trg_inactive   = 116;
constexpr int isc_dyn_trg_msg_number = 117;
constexpr int isc_dyn_trg_msg        = 118;

/**************************************/
/* Security Class specific attributes */
/**************************************/

constexpr int isc_dyn_scl_acl             = 121;
constexpr int isc_dyn_grant_user          = 130;
constexpr int isc_dyn_grant_user_explicit = 219;
constexpr int isc_dyn_grant_proc          = 186;
constexpr int isc_dyn_grant_trig          = 187;
constexpr int isc_dyn_grant_view          = 188;
constexpr int isc_dyn_grant_options       = 132;
constexpr int isc_dyn_grant_user_group    = 205;
constexpr int isc_dyn_grant_role          = 218;


/**********************************/
/* Dimension specific information */
/**********************************/

constexpr int isc_dyn_dim_lower = 141;
constexpr int isc_dyn_dim_upper = 142;

/****************************/
/* File specific attributes */
/****************************/

constexpr int isc_dyn_file_name          = 125;
constexpr int isc_dyn_file_start         = 126;
constexpr int isc_dyn_file_length        = 127;
constexpr int isc_dyn_shadow_number      = 128;
constexpr int isc_dyn_shadow_man_auto    = 129;
constexpr int isc_dyn_shadow_conditional = 130;

/********************************/
/* Log file specific attributes */
/********************************/
/* Deprecated.
#define isc_dyn_log_file_sequence         177
#define isc_dyn_log_file_partitions       178
#define isc_dyn_log_file_serial           179
#define isc_dyn_log_file_overflow         200
#define isc_dyn_log_file_raw		  201
*/

/***************************/
/* Log specific attributes */
/***************************/
/* Deprecated.
#define isc_dyn_log_group_commit_wait     189
#define isc_dyn_log_buffer_size           190
#define isc_dyn_log_check_point_length    191
#define isc_dyn_log_num_of_buffers        192
*/

/********************************/
/* Function specific attributes */
/********************************/

constexpr int isc_dyn_function_name          = 145;
constexpr int isc_dyn_function_type          = 146;
constexpr int isc_dyn_func_module_name       = 147;
constexpr int isc_dyn_func_entry_point       = 148;
constexpr int isc_dyn_func_return_argument   = 149;
constexpr int isc_dyn_func_arg_position      = 150;
constexpr int isc_dyn_func_mechanism         = 151;
constexpr int isc_dyn_filter_in_subtype      = 152;
constexpr int isc_dyn_filter_out_subtype     = 153;
constexpr int isc_dyn_description2		     = 154;
constexpr int isc_dyn_fld_computed_source2	 = 155;
constexpr int isc_dyn_fld_edit_string2	     = 156;
constexpr int isc_dyn_fld_query_header2	     = 157;
constexpr int isc_dyn_fld_validation_source2 = 158;
constexpr int isc_dyn_trg_msg2		         = 159;
constexpr int isc_dyn_trg_source2		     = 160;
constexpr int isc_dyn_view_source2	         = 161;
constexpr int isc_dyn_xcp_msg2		         = 184;

/*********************************/
/* Generator specific attributes */
/*********************************/

constexpr int isc_dyn_generator_name = 95;
constexpr int isc_dyn_generator_id   = 96;

/*********************************/
/* Procedure specific attributes */
/*********************************/

constexpr int isc_dyn_prc_inputs  = 167;
constexpr int isc_dyn_prc_outputs = 168;
constexpr int isc_dyn_prc_source  = 169;
constexpr int isc_dyn_prc_blr     = 170;
constexpr int isc_dyn_prc_source2 = 171;

/*********************************/
/* Parameter specific attributes */
/*********************************/

constexpr int isc_dyn_prm_number = 138;
constexpr int isc_dyn_prm_type   = 139;

/********************************/
/* Relation specific attributes */
/********************************/

constexpr int isc_dyn_xcp_msg = 185;

/**********************************************/
/* Cascading referential integrity values     */
/**********************************************/
constexpr int isc_dyn_foreign_key_update  = 205;
constexpr int isc_dyn_foreign_key_delete  = 206;
constexpr int isc_dyn_foreign_key_cascade = 207;
constexpr int isc_dyn_foreign_key_default = 208;
constexpr int isc_dyn_foreign_key_null    = 209;
constexpr int isc_dyn_foreign_key_none    = 210;

/***********************/
/* SQL role values     */
/***********************/
constexpr int isc_dyn_def_sql_role        = 211;
constexpr int isc_dyn_sql_role_name       = 212;
constexpr int isc_dyn_grant_admin_options = 213;
constexpr int isc_dyn_del_sql_role        = 214;
/* 215 & 216 are used some lines above. */

/**********************************************/
/* Generators again                           */
/**********************************************/

constexpr int isc_dyn_delete_generator  = 217;

// New for comments in objects.
constexpr int isc_dyn_mod_function      = 224;
constexpr int isc_dyn_mod_filter        = 225;
constexpr int isc_dyn_mod_generator     = 226;
constexpr int isc_dyn_mod_sql_role      = 227;
constexpr int isc_dyn_mod_charset       = 228;
constexpr int isc_dyn_mod_collation     = 229;
constexpr int isc_dyn_mod_prc_parameter = 230;

/***********************/
/* collation values     */
/***********************/
constexpr int isc_dyn_def_collation						= 231;
constexpr int isc_dyn_coll_for_charset					= 232;
constexpr int isc_dyn_coll_from							= 233;
constexpr int isc_dyn_coll_attribute					= 234;
constexpr int isc_dyn_coll_specific_attributes_charset	= 235;
constexpr int isc_dyn_coll_specific_attributes			= 236;
constexpr int isc_dyn_del_collation						= 237;

/****************************/
/* Last $dyn value assigned */
/****************************/

constexpr int isc_dyn_last_dyn_value = 237;

/******************************************/
/* Array slice description language (SDL) */
/******************************************/

constexpr int isc_sdl_version1      = 1;
constexpr int isc_sdl_eoc           = 255;
constexpr int isc_sdl_relation      = 2;
constexpr int isc_sdl_rid           = 3;
constexpr int isc_sdl_field         = 4;
constexpr int isc_sdl_fid           = 5;
constexpr int isc_sdl_struct        = 6;
constexpr int isc_sdl_variable      = 7;
constexpr int isc_sdl_scalar        = 8;
constexpr int isc_sdl_tiny_integer  = 9;
constexpr int isc_sdl_short_integer = 10;
constexpr int isc_sdl_long_integer  = 11;
constexpr int isc_sdl_literal       = 12;
constexpr int isc_sdl_add           = 13;
constexpr int isc_sdl_subtract      = 14;
constexpr int isc_sdl_multiply      = 15;
constexpr int isc_sdl_divide        = 16;
constexpr int isc_sdl_negate        = 17;
constexpr int isc_sdl_eql           = 18;
constexpr int isc_sdl_neq           = 19;
constexpr int isc_sdl_gtr           = 20;
constexpr int isc_sdl_geq           = 21;
constexpr int isc_sdl_lss           = 22;
constexpr int isc_sdl_leq           = 23;
constexpr int isc_sdl_and           = 24;
constexpr int isc_sdl_or            = 25;
constexpr int isc_sdl_not           = 26;
constexpr int isc_sdl_while         = 27;
constexpr int isc_sdl_assignment    = 28;
constexpr int isc_sdl_label         = 29;
constexpr int isc_sdl_leave         = 30;
constexpr int isc_sdl_begin         = 31;
constexpr int isc_sdl_end           = 32;
constexpr int isc_sdl_do3           = 33;
constexpr int isc_sdl_do2           = 34;
constexpr int isc_sdl_do1           = 35;
constexpr int isc_sdl_element       = 36;

/********************************************/
/* International text interpretation values */
/********************************************/

constexpr int isc_interp_eng_ascii = 0;
constexpr int isc_interp_jpn_sjis  = 5;
constexpr int isc_interp_jpn_euc   = 6;

/*****************/
/* Blob Subtypes */
/*****************/

/* types less than zero are reserved for customer use */

constexpr int isc_blob_untyped = 0;

/* internal subtypes */

constexpr int isc_blob_text                   = 1;
constexpr int isc_blob_blr                    = 2;
constexpr int isc_blob_acl                    = 3;
constexpr int isc_blob_ranges                 = 4;
constexpr int isc_blob_summary                = 5;
constexpr int isc_blob_format                 = 6;
constexpr int isc_blob_tra                    = 7;
constexpr int isc_blob_extfile                = 8;
constexpr int isc_blob_max_predefined_subtype = 9;

/* the range 20-30 is reserved for dBASE and Paradox types */

constexpr int isc_blob_formatted_memo = 20;
constexpr int isc_blob_paradox_ole    = 21;
constexpr int isc_blob_graphic        = 22;
constexpr int isc_blob_dbase_ole      = 23;
constexpr int isc_blob_typed_binary   = 24;

/* Deprecated definitions maintained for compatibility only */

constexpr int isc_info_db_SQL_dialect    = 62;
constexpr int isc_dpb_SQL_dialect        = 63;
constexpr int isc_dpb_set_db_SQL_dialect = 65;


#include "iberror.h"

#endif /* JRD_IBASE_H */

