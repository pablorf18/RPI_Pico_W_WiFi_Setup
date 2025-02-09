#ifndef WEB_HANDLERS_SSI_H
#define WEB_HANDLERS_SSI_H

/**
 * @file SSI.h
 * @brief Server Side Includes, used to resolve dynamic content in the server
**/

#include "CGI.h"

// SSI tags
static const char * kSSITags[] = {"wifi"};

/**
 * @brief SSI handler function
 * @param iIndex The index of the SSI tag
 * @param pcInsert The buffer to insert the content
 * @param iInsertLen The length of the buffer
 * @return The length of the inserted content
*/
u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen);

/**
 * @brief Initialize the SSI handlers
*/
void SSIInit();

#endif // WEB_HANDLERS_SSI_H