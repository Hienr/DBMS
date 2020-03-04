#ifndef W_EXCEPTIONS_H
#define W_EXCEPTIONS_H

/* Enums for exception handeling */

enum SQL_exceptions { INVALID_INPUT, INVALID_COMMAND, ECHO_INPUT, STREAM_EOF,
                      TERMINATE_PROGRAM , INVALID_BATCH_FILE, FIELDS_MISMATCH,
                      UNABLE_TO_PROCESS, TERMINATE, FILE_DNE, UNABLE_TO_EVAL,
                      VALUES_MISMATCH
                    };

#endif // W_EXCEPTIONS_H
