/*
 Le code source pr�sent sur cette page est utilisable � volont�, m�me pour utilisation commerciale.
 La reproduction de l'article est autoris�e avec l'accord de l'auteur.
 */


//#include "main.h"
#include "fonctions.h"
 //"btBulletDynamicsCommon.h"

#include "/Users/Ulala/develop/Kapla_graphic_part/Kapla graphique/BulletDynamics.framework/Versions/2.78/Headers/btBulletDynamicsCommon.h"
// Notre application SFML
sf::Window	Application;

// Ensuite on d�clare le nom du monde physique
btDiscreteDynamicsWorld* myWorld;

// La classe btBroadphaseInterface fournit une interface pour d�tecter les objets o� leurs AABB se chevauchent.
btBroadphaseInterface*	myBroadphase;

// btCollisionDispatcher supporte des algorithmes qui peuvent g�rer des pairs de collisions ConvexConvex et ConvexConcave. Temps de l'impact, le point le plus proche et p�n�tration de profondeur.
btCollisionDispatcher*	myDispatcher;

// btCollisionConfiguration permet de configurer les allocataires de m�moire.
btDefaultCollisionConfiguration* myCollisionConfiguration;

// btSequentialImpulseConstraintSolver est une impl�mentation SIMD rapide de la m�thode Projected Gauss Seidel (iterative LCP).
btSequentialImpulseConstraintSolver *mySequentialImpulseConstraintSolver;

// Position, orientation.
btTransform myTransform;

// btDefaultMotionState fournit une impl�mentation pour synchroniser les transformations.
btDefaultMotionState *myMotionState,
*myMotionState_Sol;

// Une matrice OpenGL, pour r�cup�rer la position, rotation d'un objet.
btScalar	matrix[16];

// Le corps d'une boite et de notre sol.
// btRigidBody est la classe principale des objets rigides
btRigidBody *body_kapla,
*body_sol;



using namespace std;

int main ( int argc, char **argv )
{
	// On cr�e notre fen�tre gr�ce � SFML
	Application.Create( sf::VideoMode( 800, 500, 32 ), "SFML : Bullet physics", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
    //Application.Create(sf::VideoMode::GetMode(0), "SFML Window", sf::Style::Fullscreen);
	// Creation d'une fen�tre plein �cran avec le meilleur mode vid�o
  	
    
    
    /// Bullet physics
	
    ///collision configuration contains default setup for memory, collision setup
	myCollisionConfiguration = new btDefaultCollisionConfiguration();
	
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	myDispatcher = new	btCollisionDispatcher(myCollisionConfiguration);
	
	myBroadphase = new btDbvtBroadphase();
	
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mySequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;
	
	// initialisation du monde bullet
	myWorld = new btDiscreteDynamicsWorld(myDispatcher,myBroadphase,mySequentialImpulseConstraintSolver,myCollisionConfiguration);
	
	// On d�finit la gravit�, de fa�on � ce que les objets tombent vers le bas (-Y).
	myWorld->setGravity( btVector3(0,-10,0) );
	
	/// SOL ///////////////////////////////////////////
	// create a shape
	btCollisionShape* shape_sol = new btBoxShape( btVector3(100,1,100) );
	
	myTransform.setIdentity();
	myTransform.setOrigin( btVector3(0,0,0) );
	
	btVector3 localInertiaSol(0,0,0);
	
	btScalar mass = 0;
	
	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState_Sol = new btDefaultMotionState(myTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo_sol( mass, myMotionState_Sol, shape_sol, localInertiaSol );
	body_sol = new btRigidBody(rbInfo_sol);
	
	// Add the body to the dynamics world
	myWorld->addRigidBody(body_sol);

 
    
    
    
    // Create a clock for measuring time elapsed
    sf::Clock montre;
    //Variable pour calculer le delta de d�placement de la souris quand les clicks droit et gauche de la souris sont enfonc� pour manipuler la cam�ra
  
    // Pour d�clancher la chute d'un seul kapla quand la touche Espace est relach�e
	bool trigger = false; 
 
	unsigned int windowsWidth = Application.GetWidth();
    unsigned int windowsHeight = Application.GetHeight();
    
    
    int startPointX(0),startPointY(0);
    float deltaX(0),deltaY(0),prevDeltaX(1),prevDeltaY(1);
    int   MouseX(0);
    int   MouseY(0);
    bool show = true;
    
	//unsigned int sizeWidth = sf::Window::GetWidth();
    
    //On initialise une cam�ra qui sera plac� par d�faut par le constructeur
    Camera camcam(110,60.0,60.0);
    Cursor cursor;
bool test = false;
       
    float time;

 // pour avoir les infos clavier en temps r�el
	const sf::Input& Input = Application.GetInput();
	// Notre boucle d'affichage
	while(Application.IsOpened() )
	{
        Application.ShowMouseCursor (false);
        // r�f�rence vers l'entr�e associ�e � une fen�tre (pour r�cup�rer les donn�s clavier en temps r�el
        
        sf::Event Event;
        //Utilise les fl�che pour d�placer le Kapla qui va �tre lach�
        // Get some useless input states, just to illustrate the tutorial
        bool         LeftKeyDown     = Input.IsKeyDown(sf::Key::Left);
        bool         RightKeyDown     = Input.IsKeyDown(sf::Key::Right);
        bool         UpKeyDown     = Input.IsKeyDown(sf::Key::Up);
        bool         DownKeyDown     = Input.IsKeyDown(sf::Key::Down);
        
        bool         RightButtonDown = Input.IsMouseButtonDown(sf::Mouse::Right);
        bool         LeftButtonDown = Input.IsMouseButtonDown(sf::Mouse::Left);
        bool         Espace     = Input.IsKeyDown(sf::Key::Space);
        bool         Shift     = Input.IsKeyDown(sf::Key::LShift);
        MouseX  =   Input.GetMouseX() ;
        MouseY  =   Input.GetMouseY() ;
        
        if (LeftButtonDown) 
        {
                if (show ) 
                {
                    montre.Reset();
                    startPointY = MouseY;
                    startPointX = MouseX;
                //cout<< "start point" <<startPointX<< endl;
                    
                    show = false;
                }
                time = montre.GetElapsedTime();
            //cout<< time << endl;
           
                if (time > 0.02) 
                {
                
                    deltaX = ((MouseX-startPointX));
                
                    deltaY = ((MouseY-startPointY));
                    //show = true;
                cout<< deltaX<< endl;
                    if ((prevDeltaX != deltaX)||(prevDeltaY != deltaY)) {
                        camcam.tumble(deltaX/5,deltaY/5);
                        show = true;
                        //montre.Reset();
                        prevDeltaX = deltaX;
                        prevDeltaY = deltaY;
                        
                    }
            }
            
        }else
        {
            show = true;
            cursor.set((MouseX - windowsWidth/2), (MouseY - windowsHeight/2));
        }
        
        if (Shift) {
            test=true; 
        }else{
            test=false;
        }
        
        

        if (LeftKeyDown) {
            camcam.tumble(0.005, 0);
            LeftKeyDown = false;
        }
        if (RightKeyDown) {
            camcam.tumble(-0.005, 0);
            RightKeyDown = false;
        }
        if (UpKeyDown) {
            camcam.tumble(0, 0.005);
        }
        if (DownKeyDown) {
            camcam.tumble(0, -0.005);
        }
        //unsigned int delta =mouseT3 - mouseT1;
        //cout << delta<< endl;
        
        
        
        
        while (Application.GetEvent(Event))
        {
            
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
            // Fen�tre ferm�e
            if (Event.Type == sf::Event::Closed)
                Application.Close();
            
            // Touche 'echap' appuy�e
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                Application.Close();
            if ((Event.Type == sf::Event::KeyReleased) && (Event.Key.Code == sf::Key::Space)) {
                /// box ///////////////////////////////////////////
                // create a shape
                btCollisionShape* shape_kapla = new btBoxShape( btVector3(3,1,15) );
                
                myTransform.setIdentity();
                int dropX((MouseX - windowsWidth/2));
                int dropY((MouseY - windowsHeight/2));
                myTransform.setOrigin(btVector3(dropX,5,dropY));
                
                btVector3 localInertia(0,0,0);
                
                mass = 0.5f;
                
                shape_kapla->calculateLocalInertia( mass, localInertia );
                
                //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                myMotionState = new btDefaultMotionState(myTransform);
                btRigidBody::btRigidBodyConstructionInfo myBoxRigidBodyConstructionInfo( mass,myMotionState, shape_kapla, localInertia );
                body_kapla = new btRigidBody(myBoxRigidBodyConstructionInfo);
                
                //add the body to the dynamics world
                
                myWorld->addRigidBody(body_kapla);
                trigger=true;
            }
        }

        Application.SetActive();
         
        
        

        
        //NEED convertiseur coord souris -> coord du plan visible par la cam�ra
        // le d�placement sur le plan de la fen�tre est proportionel � celui du plan de construction
        
        // dessin  le curseur
        //cursor.drawKapla(1,15,3); 
        
        
        //touche espace enfonc�e et relach�e
       if ((Espace))
        {
           
         
        }              
                                   
        if (myWorld)
		{
			myWorld->stepSimulation( 0.0001 );
		}

       
        camcam.display();
        
        cursor.drawKapla(15, 3, 1);
        // On recup�re la matrice OpenGL transform�e par Bullet qu'on appliquera � notre boite
        if (trigger)
        {
                myMotionState->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
            glPushMatrix();
            glMultMatrixf( matrix );
            box(3,1,15);
            glPopMatrix();
            
        }
        
        
		box(100,1,100);
		// On a	ffiche le sol;
        if (test==true)
        {
                   }
		
		
        
		
		// swap buffers, etc
		Application.Display();
        
	}
}
