/* **************************************************************************
   Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/



#include "target.h"
#include "board.h"
#include "protocol.h"

#include "boot_api.h"
#include "bootloader.h"

int main() 
{
	  hw_init();

    board_init ();

    // **** test
    //run_protocol();
    // ****

    if (board_button_pressed() || bca_download_request())
    {
      board_notify (bn_bootloader_requested);
    }
    else 
    {
        if (user_code_present()) 
        {
          // user code valid
          board_notify (bn_user_code_valid);

            // starting user code...
          board_notify (bn_jumping_to_user_code);
          initialise_bca();
          bca_set_response (BOOT_RES_NORMAL_BOOT);
            execute_user_code();
        }
        else 
        {
            // user code invalid
          board_notify (bn_user_code_invalid);
        }
    }

          // enter bootloader
    initialise_bca();
          board_notify (bn_protocol_idle);
    if (run_protocol())
    {
      board_notify (bn_jumping_to_user_code);
      bca_set_response (BOOT_RES_FIRMWARE_UPLOADED);
      execute_user_code();
        }
    else
    {
      // what to do here?
      hw_reboot();
    }        
}
