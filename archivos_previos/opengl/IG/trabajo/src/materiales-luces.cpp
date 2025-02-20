// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#include "matrices-tr.h"
#include "materiales-luces.h"

using namespace std ;

const bool trazam = false ;

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETAR: práctica 4: cargar imagen de textura
   // (las variables de instancia están inicializadas en la decl. de la clase)
   // .....
	char cstr[nombreArchivoJPG.size() + 1];
	nombreArchivoJPG.copy(cstr, nombreArchivoJPG.size() + 1);
	cstr[nombreArchivoJPG.size()] = '\0';
	imagen = LeerArchivoJPEG(cstr, ancho, alto);

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // y configurar parámetros de la textura (glTexParameter)
   // .......
   	if (enviada){
   		cout << "Ya se ha enviado" <<endl;
   		return;	
   	}
   	
   	
	glGenTextures( 1, &ident_textura );
	glBindTexture(GL_TEXTURE_2D,ident_textura);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, ancho, alto, GL_RGB, GL_UNSIGNED_BYTE, imagen);
	
	enviada = true;

}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != nullptr )
      delete [] imagen ;

   imagen = nullptr ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------new Textura("../recursos/imgs/text-madera.jpg")
// por ahora, se asume la unidad de texturas #0

void Textura::activar( Cauce & cauce  )
{
   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......
   	glColor3f(1.0,1.0,1.0);
   
   	CError();
   	if (!enviada){
   		enviar();
   	}
   	
   	cauce.fijarEvalText(enviada, ident_textura);
   	cauce.fijarTipoGCT(modo_gen_ct, coefs_s, coefs_t);
	CError();
}
// *********************************************************************
// crea un material usando un color plano y los coeficientes de las componentes

Material::Material( const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = nullptr ;
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}

//----------------------------------------------------------------------
// crea un material usando una textura y los coeficientes de las componentes

Material::Material( Textura * p_textura, const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = p_textura ;  assert( textura != nullptr );
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( textura != nullptr )
   {  delete textura ;
      textura = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar( Cauce & cauce )
{
   // COMPLETAR: práctica 4: activar un material
   // .....
   	CError();
   	if (textura != nullptr){
   		textura->activar(cauce);
   	}else{
   		cauce.fijarEvalText(false);
   	}
   	CError();
   	cauce.fijarParamsMIL(Tupla3f(k_amb,k_amb,k_amb), Tupla3f(k_dif,k_dif,k_dif), Tupla3f(k_pse,k_pse,k_pse), exp_pse);
   	

}
//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const Tupla3f & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi     = longi_ini ;
   lati      = lati_ini ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   //ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}
//----------------------------------------------------------------------

bool FuenteLuz::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}
//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLongi( const float incre )
{
   longi = longi + incre ;
   using namespace std ;
   cout << "incrementada longitud de una fuente de luz, nueva == " << longi << endl ;
}
//----------------------------------------------------------------------
// para fuentes diraccionales, incrementar o decrementar la longitud
// (en las puntuales no hace nada)
void FuenteLuz::actualizarLati( const float incre )
{
   lati = lati + incre ;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   //pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( Cauce & cauce )
{
   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   //   (crear un array con los colores y otro con las posiciones/direcciones,
   //    usar el cauce para activarlas)
   // .....
   
   std::vector<Tupla3f> colores ;
   std::vector<Tupla4f> pos_dir ;
   
   for (int i=0; i< vpf.size(); i++){
   	float lat = (vpf[i]->lati)*M_PI/180.0;
   	float lon = (vpf[i]->longi)*M_PI/180.0;
   	colores.push_back(vpf[i]->col_difuso);
   	pos_dir.push_back(Tupla4f(cos(lon),sin(lat),sin(lon),0.0).normalized());
   }
   
   cauce.fijarFuentesLuz(colores, pos_dir);

}

// ---------------------------------------------------------------------
// pasa a la siguiente fuente de luz (si d==+1, o a la anterior (si d==-1))
// aborta si 'd' no es -1 o +1

void ColFuentesLuz::sigAntFuente( int d )
{
   assert( i_fuente_actual < vpf.size()) ;
   assert( d == 1 || d== -1 );
   i_fuente_actual = unsigned((int(i_fuente_actual+vpf.size())+d) % vpf.size()) ;
   cout << "fuente actual: " << (i_fuente_actual+1) << " / " << vpf.size() << endl ;
}

// ---------------------------------------------------------------------
// devuelve un puntero a la fuente de luz actual

FuenteLuz * ColFuentesLuz::fuenteLuzActual()
{
   assert( vpf[i_fuente_actual] != nullptr );
   return vpf[i_fuente_actual] ;
}
//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}

//--------------------------------------------------------------------------
// actualizar una colección de fuentes en función de una tecla GLFW pulsada
// (se usa el código glfw de la tecla, se llama desde 'main.cpp' con L pulsada)
// devuelve 'true' sii se ha actualizado algo

bool ProcesaTeclaFuenteLuz( ColFuentesLuz * col_fuentes, int glfw_key )
{
   assert( col_fuentes != nullptr );

   FuenteLuz * fuente     = col_fuentes->fuenteLuzActual() ; assert( fuente != nullptr );
   bool        redib      = true ;
   const float delta_grad = 2.0 ; // incremento en grados para long. y lati.

   switch( glfw_key )
   {
      case GLFW_KEY_KP_ADD :
         col_fuentes->sigAntFuente( +1 );
         break ;
      case GLFW_KEY_KP_SUBTRACT :
         col_fuentes->sigAntFuente( -1 );
         break ;
      case GLFW_KEY_LEFT :
         fuente->actualizarLongi( +delta_grad );
         break ;
      case GLFW_KEY_RIGHT :
         fuente->actualizarLongi( -delta_grad );
         break ;
      case GLFW_KEY_DOWN :
         fuente->actualizarLati( -delta_grad );
         break ;
      case GLFW_KEY_UP :
         fuente->actualizarLati( +delta_grad );
         break ;
      default :
         redib = false ;
         break ;
   }
   return redib ;
}

//-----------------------------------------------------------------------
// constructor de una colección de fuentes de luz sencilla que incluye
// dos fuentes de luz.

Col2Fuentes::Col2Fuentes()
{
   const float f0 = 0.7, f1 = 0.3 ;
   insertar( new FuenteLuz( +45.0, 60.0,  Tupla3f { f0, f0,     f0,    } ) );
   insertar( new FuenteLuz( -70.0, -30.0, Tupla3f { f1, f1*0.5, f1*0.5 } ) );

}
