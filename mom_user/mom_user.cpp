//////////////////////////////////////////////////////////////////////////////
//
//  mom_user.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

// #define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <uf.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_mom.h>

#include <windows.h>

#include "mom_user.h"

#define GRADTORAD1  0.0174532925199433    // PI/180.0
#define GRADTORAD2  0.008726646259971648  // PI/360.0


//----------------------------------------------------------------------------

/*
 * Защита через номер тома:
 */
int sub_10002920() ;
/*
 * Exported Tcl procedures:
 */
static int sub_10001090
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
);

//----------------------------------------------------------------------------
//  Activation Methods
//----------------------------------------------------------------------------

//  Explicit Activation
//      This entry point is used to activate the application explicitly, as in
//      "File->Execute UG/Open->User Function..."
//----- (10001010) --------------------------------------------------------
extern "C" DllExport void entry_rtv_mom_cp_part_attr( char *parm, int *returnCode, int rlen )
{
  int errorCode ;
  UF_MOM_id_t mom ;
  void *interp = NULL ;

  if ( !UF_is_initialized() )
  {
    errorCode = UF_initialize();
    *returnCode = errorCode;
    if ( errorCode ) return ;
  }

  /* Get the TCL interpreter id from the ufusr param */
  UF_MOM_ask_interp_from_param ( parm, &interp ) ;
  /* Get the MOM id from the ufusr param */
  UF_MOM_ask_mom (parm, &mom) ;
  /*Создаем функции в интепретаторе*/
  UF_MOM_extend_xlator ( mom , "EXTN_rtv_mom_cp_part_attr", sub_10001090 ) ;

  *returnCode = 0;
  if ( errorCode == 1 )  errorCode = UF_terminate();
  *returnCode = errorCode;
}


//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

// Unload Handler
//     This function specifies when to unload your application from Unigraphics.
//     If your application registers a callback (from a MenuScript item or a
//     User Defined Object for example), this function MUST return
//     "UF_UNLOAD_UG_TERMINATE".
extern "C" int ufusr_ask_unload( void )
{
     /* unload immediately after application exits*/
     //return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     //return ( UF_UNLOAD_SEL_DIALOG );
     /*when UG terminates...              */
     return ( UF_UNLOAD_UG_TERMINATE );
}


/*-----------------------------------------------------------------------
 * Function Name: sub_10001090
 *
 *
 -----------------------------------------------------------------------*/
static int sub_10001090
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
)
{
  UF_MOM_id_t mom_id = ( UF_MOM_id_t ) client_data ;

  int     ii ;
  char    Dest[256] ;
  double  angle1 , angle2 ;
  double  prev_angle1, prev_angle2 ;
  double  x , y , z , i , j , k ;
  double  prev_x , prev_y , prev_z ;
  double  h;
  double  dT ;
  int     iT ;
  const char *String , *StringP_num;
  double  P_num;
  int     iP_num;
  const char *kin_retract_distance ;
  double  retract_distance ;
  double  dx , dy , dz ;

  double  var_0, var_1, var_2 , var_3 , var_4  ;

  if ( sub_10002920() == 0 )
  {
    const int Size = 1024 ;
    char  str[Size]="НА ЭТОМ РАБОЧЕМ МЕСТЕ ПОСТПРОЦЕССОР НЕ МОЖЕТ БЫТЬ ЗАПУЩЕН\n \
Москва, CSoft (ЗАО \"СиСофт\"),\nТелефон: +7 (495) 913-22-22, Факс: +7 (495) 913-22-21\n \
e-mail: jura@csoft.ru, g00@inbox.ru\nHTTP: http://www.csoft.ru\n\n(c) 2007.\n\n\nПоследняя редакция 2008\0";

    UF_UI_display_nonmodal_msg("Внимание!", str, UF_UI_MSG_POS_CURSOR);
    UF_terminate();
    return ( 0 ) ;
  }

    sprintf(Dest, "Number of args %ld", argc);
    if ( argc > 0 )
    {
      for (ii=0;ii<argc;ii++) sprintf(Dest, "Arg 0 %s", argv[ii]);
    }

    if (0==strcmp(argv[1],"PLANE_VORTEX"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);

      var_2=cos(angle2*GRADTORAD1);
      var_1=sin(angle1*GRADTORAD1);

      var_3=cos(var_1);
      var_4=sin(var_3);

      // #600=[[#[2000+#1034]+360.051]*[%.8lf]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043600=\\133\\133\\043\\1332000+\\0431034\\135+360.051\\135*\\133%.8lf\\135\\135\"",
        var_4);
      UF_MOM_execute_command(mom_id, Dest);
      // #601=[[#[2000+#1034]+360.051]*[1-%.8lf]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043601=\\133\\133\\043\\1332000+\\0431034\\135+360.051\\135*\\1331-%.8lf\\135\\135\"",
        var_2);
      UF_MOM_execute_command(mom_id, Dest);
      // #602=[[#[2000+#1034]+360.000]*[%.8lf]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043602=\\133\\133\\043\\1332000+\\0431034\\135+360.000\\135*\\133%.8lf\\135\\135\"",
        var_1);
      UF_MOM_execute_command(mom_id, Dest);
      // #603=[[#[2000+#1034]+360.000]*[1-%.8lf]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043603=\\133\\133\\043\\1332000+\\0431034\\135+360.000\\135*\\1331-%.8lf\\135\\135\"",
        var_3);
      UF_MOM_execute_command(mom_id, Dest);
      // G68 X[#600] Y0 Z[-#601] I0 J1 K0 R%.3lf
      sprintf(
        Dest,
        "MOM_output_literal \"G68 X\\133\\043600\\135 Y0 Z\\133-\\043601\\135 I0 J1 K0 R%.3lf \"",
        angle2);
      UF_MOM_execute_command(mom_id, Dest);

      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);

      // G68 X0 Y[-#602] Z[-#603] I1 J0 K0 R%.3lf
      sprintf(
        Dest,
        "MOM_output_literal \"G68 X0 Y\\133-\\043602\\135 Z\\133-\\043603\\135 I1 J0 K0 R%.3lf \"",
        angle1);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"G43_MOVE_VORTEX"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      UF_MOM_ask_string(mom_id, "tool_number", &String);
      iT = (int)atof(String);
      sprintf(Dest, "MOM_output_literal \"G43.4 G0 X%.3lf Y%.3lf H%.d\"", x, y, z, iT);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G49\"");
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G43 G91 Z0 H%.d\"", iT);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G90\"");
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"G43_MOVE_VORTEX_%"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      UF_MOM_ask_string(mom_id, "P_num", &StringP_num);
      iP_num = (int) atof(StringP_num);

    }

    if (0==strcmp(argv[1],"PLANE"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      sprintf(
        Dest,
        "MOM_output_literal \"G65 P2223 A%.3lf C%.3lf D%.3lf E%.3lf\"",
        angle1, angle2 , prev_angle1 , prev_angle2 );
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_dynamic"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      UF_MOM_ask_string(mom_id, "P_num", &StringP_num);
      iP_num = (int) atof(StringP_num);
      sprintf(Dest, "MOM_output_literal \"G128 A%.3lf C%.3lf Q%.d\"", angle1, angle2, iP_num);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"G43_MOVE"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      UF_MOM_ask_string(mom_id, "P_num", &StringP_num);
      iP_num = (int) atof(StringP_num);

      var_1=cos(angle1*GRADTORAD1);

      // #600=[[#[60000+#3020]+299.862]*[%.8lf]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043600=\\133\\133\\043\\13360000+\\0433020\\135+299.862\\135*\\133%.8lf\\135\\135\"",
        var_1);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_MILL2"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      UF_MOM_ask_string(mom_id, "P_num", &StringP_num);
      iP_num = (int) atof(StringP_num);

      var_0=angle1*GRADTORAD2 ;
      var_1=sin(var_0);
      var_2=cos(var_1);

      // #600=[[#[60000+#3020]+299.862]*[%.8lf*2]]
      sprintf(
        Dest,
        "MOM_output_literal \"\\043600=\\133\\133\\043\\13360000+\\0433020\\135+299.862\\135*\\133%.8lf*2\\135\\135\"",
        var_1);
      UF_MOM_execute_command(mom_id, Dest);
      // #810=[#600*[%.8lf]]
      sprintf(Dest, "MOM_output_literal \"\\043810=\\133\\043600*\\133%.8lf\\135\\135\"", var_2);
      UF_MOM_execute_command(mom_id, Dest);
      // #812=[#600*[%.8lf]]
      sprintf(Dest, "MOM_output_literal \"\\043812=\\133\\043600*\\133%.8lf\\135\\135\"", var_1);
      UF_MOM_execute_command(mom_id, Dest);

      if ( var_0==0.0 )
      {
        // G0 C[#818+%.3lf]
        sprintf(Dest, "MOM_output_literal \"G0 C\\133\\043818+%.3lf\\135\"", angle2);
        UF_MOM_execute_command(mom_id, Dest);
      }
      if ( iP_num <= 0 || iP_num >= 49 )
      {
        // G65 P9998 C[#818+%.3lf] Q0
        sprintf(Dest, "MOM_output_literal \"G65 P9998 C\\133\\043818+%.3lf\\135 Q0\"", angle2);
        UF_MOM_execute_command(mom_id, Dest);
      }
      else
      {
        // G65 P9998 C[#818+%.3lf] Q%.d
        sprintf(
          Dest,
          "MOM_output_literal \"G65 P9998 C\\133\\043818+%.3lf\\135 Q%.d\"",
          angle2,
          iP_num);
        UF_MOM_execute_command(mom_id, Dest);
      }
      // G68 X[#810+#800] Y[#801] Z[-#812] I0 J0 K1 R[-#818]
      sprintf(
        Dest,
        "MOM_output_literal \"G68 X\\133\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J0 K1 R\\133-\\043818\\135\"");
      UF_MOM_execute_command(mom_id, Dest);

      if ( var_0<0 )
      {
        // G68 X[-#810+#800] Y[#801] Z[-#812] I0 J1 K0 R%.3lf
        sprintf(
          Dest,
          "MOM_output_literal \"G68 X\\133-\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J1 K0 R%.3lf\"",
          angle1);
      }
      else
      {
        // G68 X[#810+#800] Y[#801] Z[-#812] I0 J1 K0 R%.3lf
        sprintf(
          Dest,
          "MOM_output_literal \"G68 X\\133\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J1 K0 R%.3lf\"",
          angle1);
      }
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G0 X%.3lf Y%.3lf\"", x, y);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G69\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G68 X[#800] Y[#801] Z0 I0 J0 K1 R[-#818]
      sprintf(
        Dest,
        "MOM_output_literal \"G68 X\\133\\043800\\135 Y\\133\\043801\\135 Z0 I0 J0 K1 R\\133-\\043818\\135\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G68 X[#800] Y[#801] Z0 I0 J1 K0 R%.3lf
      sprintf(
        Dest,
        "MOM_output_literal \"G68 X\\133\\043800\\135 Y\\133\\043801\\135 Z0 I0 J1 K0 R%.3lf\"",
        angle1);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_MILL2_PR"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "0", &prev_angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_out_angle_pos", "1", &prev_angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_pos", "2", &z);
      UF_MOM_ask_string(mom_id, "P_num", &StringP_num);
      iP_num = (int) atof(StringP_num);

      var_0=angle1*GRADTORAD2 ;
      var_1=sin(var_0);
      var_2=cos(var_0);

      // #600=[[#[60000+#3020]+299.862]*[%.8lf*2]]
      sprintf(
        Dest,
        "puts $ofile  \"\\043600=\\133\\133\\043\\13360000+\\0433020\\135+299.862\\135*\\133%.8lf*2\\135\\135\"",
        var_1);
      UF_MOM_execute_command(mom_id, Dest);
      // #810=[#600*[%.8lf]]
      sprintf(Dest, "puts $ofile  \"\\043810=\\133\\043600*\\133%.8lf\\135\\135\"", var_2);
      UF_MOM_execute_command(mom_id, Dest);
      // #812=[#600*[%.8lf]]
      sprintf(Dest, "puts $ofile  \"\\043812=\\133\\043600*\\133%.8lf\\135\\135\"", var_1);
      UF_MOM_execute_command(mom_id, Dest);

      if ( var_0 == 0. )
      {
        // G0 C[#818+%.3lf]
        sprintf(Dest, "puts $ofile  \"G0 C\\133\\043818+%.3lf\\135\"", angle2);
        UF_MOM_execute_command(mom_id, Dest);
      }
      if ( iP_num <= 0 || iP_num >= 49 )
      {
        // G65 P9998 C[#818+%.3lf] Q0
        sprintf(Dest, "puts $ofile  \"G65 P9998 C\\133\\043818+%.3lf\\135 Q0\"", angle2);
        UF_MOM_execute_command(mom_id, Dest);
      }
      else
      {
        // G65 P9998 C[#818+%.3lf] Q%.d
        sprintf(
          Dest,
          "puts $ofile  \"G65 P9998 C\\133\\043818+%.3lf\\135 Q%.d\"",
          angle2,
          iP_num);
        UF_MOM_execute_command(mom_id, Dest);
      }
      // G68 X[#810+#800] Y[#801] Z[-#812] I0 J0 K1 R[-#818]
      sprintf(
        Dest,
        "puts $ofile  \"G68 X\\133\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J0 K1 R\\133-\\043818\\135\"");
      UF_MOM_execute_command(mom_id, Dest);

      if ( var_0<0 )
      {
        // G68 X[-#810+#800] Y[#801] Z[-#812] I0 J1 K0 R%.3lf
        sprintf(
          Dest,
          "puts $ofile  \"G68 X\\133-\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J1 K0 R%.3lf\"",
          angle1);
      }
      else
      {
        // G68 X[#810+#800] Y[#801] Z[-#812] I0 J1 K0 R%.3lf
        sprintf(
          Dest,
          "puts $ofile  \"G68 X\\133\\043810+\\043800\\135 Y\\133\\043801\\135 Z\\133-\\043812\\135 I0 J1 K0 R%.3lf\"",
          angle1);
       }
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "puts $ofile  \"G0 X%.3lf Y%.3lf\"", x, y);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "puts $ofile  \"G69\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G68 X[#800] Y[#801] Z0 I0 J0 K1 R[-#818]
      sprintf(Dest, "puts $ofile  \"G68 X\\133\\043800\\135 Y\\133\\043801\\135 Z0 I0 J0 K1 R\\133-\\043818\\135\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G68 X[#800] Y[#801] Z0 I0 J1 K0 R%.3lf
      sprintf(
        Dest,
        "puts $ofile  \"G68 X\\133\\043800\\135 Y\\133\\043801\\135 Z0 I0 J1 K0 R%.3lf\"",
        angle1);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE2"))
    {

      // G54.2 P1 G01 X[-ABS[#5521]+315.-5.] Y[-ABS[#5522]+315.-5.] Z[-ABS[#5523]+690.-5.] A0.C0. F10000.
      sprintf(
        Dest,
        "MOM_output_literal \"G54.2 P1 G01 X\\133-ABS\\133\\0435521\\135+315.-5.\\135 Y\\133-ABS\\133\\0435522\\135+315.-5.\\135 Z\\133-ABS\\133\\0435523\\135+690.-5.\\135 A0.C0. F10000. \"");
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_END"))
    {

      // G01 G54.2 P0 G91 X[#780] Y[#781] Z[#782] F10000.
      sprintf(
        Dest,
        "MOM_output_literal \"G01 G54.2 P0 G91 X\\133\\043780\\135 Y\\133\\043781\\135 Z\\133\\043782\\135 F10000. \"");
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G90\"");
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_moving"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      UF_MOM_ask_string(mom_id, "tool_number", &String);
      dT = atof(String) + 60000;
      sprintf(Dest, "MOM_output_literal \"G54\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G01 G54.2 P1 G91 X[-#780] Y[-#781] Z[-#782 - #[60000 + #3020]] F10000.
      sprintf(
        Dest,
        "MOM_output_literal \"G01 G54.2 P1 G91 X\\133-\\043780\\135 Y\\133-\\043781\\135 Z\\133-\\043782 - \\043\\13360000 + \\0433020\\135\\135 F10000. \"");
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G90 G0 X%.3lf Y%.3lf \"", x, y);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"PLANE_moving2"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      sprintf(Dest, "MOM_output_literal \"G54\"");
      UF_MOM_execute_command(mom_id, Dest);
      // G01 G54.2 P1 G91 X[-#780] Y[-#781] Z[-#782] F10000.
      sprintf(
        Dest,
        "MOM_output_literal \"G01 G54.2 P1 G91 X\\133-\\043780\\135 Y\\133-\\043781\\135 Z\\133-\\043782\\135 F10000. \"");
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G0 G90 X%.3lf Y%.3lf\"", x, y);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"Recalc_Pos"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);
      sprintf(Dest, "MOM_output_literal \"G0 G90 X%.3lf Y%.3lf\"", x, y);
      UF_MOM_execute_command(mom_id, Dest);

    }

    if (0==strcmp(argv[1],"Recalc_Pos_G43"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "pos", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "pos", "2", &z);

      UF_MOM_set_assoc_double_array(mom_id, "calc_pos", "0", x);
      UF_MOM_set_assoc_double_array(mom_id, "calc_pos", "1", y);
      UF_MOM_set_assoc_double_array(mom_id, "calc_pos", "2", z);

    }

    if (0==strcmp(argv[1],"Recalc_Pos_G43_rap"))
    {

      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);

      UF_MOM_set_assoc_double_array(mom_id, "pos", "0", x);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "1", y);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "2", z);

    }

    if (0==strcmp(argv[1],"INPUT_RETRACT"))
    {
     int  ia4[2]={0,0},
          ip6[2] = {1,1},
          resp;
     double ra5[2] = { 100, 100 } ;
     char   menu[2][16]; //={":Отвод=",":Врезание="};
     strcpy(&menu[0][0], ":Отвод (мм)=");
     strcpy(&menu[1][0], ":Врезание (мм)=");
     //ra5[0] = *n0;
     //ra5[1] = *n1;

      UF_UI_close_listing_window();
      UF_UI_lock_ug_access(UF_UI_FROM_CUSTOM);
      resp = uc1608(":Введите значения отвода и врезания инструмента:", menu, 2, ia4, ra5, ip6);
        //*n0 = ra5[0];
        //*n1 = ra5[1];
      UF_MOM_set_double(mom_id, "kin_retract_distance", ra5[0]);
      UF_MOM_set_double(mom_id, "kin_reengage_distance", ra5[1]);
      UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);
    }

    if (0==strcmp(argv[1],"INPUT_RETRACT_DIST"))
    {
      uc1601(":Ввод дистанций отвода и врезания:", 0);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "2", &z);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "0", &i);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "1", &j);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "2", &k);
      UF_MOM_ask_string(mom_id, "kin_retract_distance", &kin_retract_distance);
      retract_distance=atof(kin_retract_distance);
      //UF_free(&kin_retract_distance);

      dx=i*retract_distance + x ;
      dy=j*retract_distance + y ;
      dz=k*retract_distance + z ;

      UF_MOM_set_assoc_double_array(mom_id, "pos", "0", dx);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "1", dy);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "2", dz);
    }

    if (0==strcmp(argv[1],"UNLOAD"))
    {
      UF_MOM_set_string(mom_id, "var_unknow", "");
      UF_terminate();
    }

  return ( 0 ) ;
}

//----- (10002920) --------------------------------------------------------
int sub_10002920()
{
  char VolumeNameBuffer[MAX_PATH]; // [sp+444h] [bp-3E8h]@1
  DWORD VolumeSerialNumber; // [sp+0h] [bp-82Ch]@1
  DWORD MaximumComponentLength; // [sp+54h] [bp-7D8h]@1
  DWORD FileSystemFlags; // [sp+58h] [bp-7D4h]@1
  char FileSystemNameBuffer[MAX_PATH]; // [sp+5Ch] [bp-7D0h]@1

  int i ;
  static DWORD sp[]=
  {  0x0BCEC3EDA , 0x1C6992FB , 0x9C21B9ED , 0x0C4DF5004 , 0x6CF7C9A2 , 0x4C732EC5 , 0x0C80B1F50 , 0x343C618F ,
     0x0D4008323 , 0x0F4A85B70 , 0x0E82A33F0,0x0FC5BF203 , 0x0F4203A3B , 0x709AC60F , 0x0E40FAFFF , 0x0CCBFB1B7 ,
     0x2CCB6526 , 0x40E5A14D , 0x0B0B89514 ,
     1 } ;

  GetVolumeInformation(
    "C:\\",
    VolumeNameBuffer,
    sizeof(VolumeNameBuffer),
    &VolumeSerialNumber,
    &MaximumComponentLength,
    &FileSystemFlags,
    FileSystemNameBuffer,
    sizeof(FileSystemNameBuffer));

  i = 0;
  while ( VolumeSerialNumber != sp[i] )
  {
    ++i;
    if ( i >= 20 ) return 0;
  }
  return 1;
}