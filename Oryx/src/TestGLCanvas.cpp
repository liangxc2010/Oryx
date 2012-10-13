// we need OpenGL headers for GLfloat/GLint types used below
#if defined(__WXMAC__) || defined(__WXCOCOA__)
#   ifdef __DARWIN__
#       include <OpenGL/gl.h>
#       include <OpenGL/glu.h>
#   else
#       include <gl.h>
#       include <glu.h>
#   endif
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

class TestGLCanvas : public wxGLCanvas
{
public:
	TestGLCanvas(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		int *gl_attrib = NULL);

	virtual ~TestGLCanvas();

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseEvent(wxMouseEvent& event);

	void InitGL();

private:
	wxGLContext* m_glRC;

	GLfloat m_xrot;
	GLfloat m_yrot;

	bool m_bInitFlag;

	wxDECLARE_NO_COPY_CLASS(TestGLCanvas);
	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// TestGLCanvas
//---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
	EVT_SIZE(TestGLCanvas::OnSize)
	EVT_PAINT(TestGLCanvas::OnPaint)
	EVT_CHAR(TestGLCanvas::OnChar)
	EVT_MOUSE_EVENTS(TestGLCanvas::OnMouseEvent)
	END_EVENT_TABLE()

	TestGLCanvas::TestGLCanvas(wxWindow *parent,
	wxWindowID id,
	int* gl_attrib)
	: wxGLCanvas(parent, id, gl_attrib)
{
	m_xrot = 0;
	m_yrot = 0;
	m_bInitFlag = false;

	// Explicitly create a new rendering context instance for this canvas.
	m_glRC = new wxGLContext(this);
}

TestGLCanvas::~TestGLCanvas()
{
	delete m_glRC;
}

void TestGLCanvas::InitGL()
{
	SetCurrent(*m_glRC);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 25.0 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, -6.0 );
}

static void DrawTri(void)
{
	glPushAttrib(GL_CURRENT_BIT);			// 保存当前的绘制属性
	glBegin(GL_TRIANGLES);
	// 前面
	glColor3f(1.0f,0.0f,0.0f);						
	glVertex3f( 0.0f, 1.0f, 0.0f);					
	glColor3f(0.0f,1.0f,0.0f);						
	glVertex3f(-1.0f,-1.0f, 1.0f);					
	glColor3f(0.0f,0.0f,1.0f);						
	glVertex3f( 1.0f,-1.0f, 1.0f);	

	// 右面
	glColor3f(1.0f,0.0f,0.0f);						
	glVertex3f( 0.0f, 1.0f, 0.0f);					
	glColor3f(0.0f,0.0f,1.0f);						
	glVertex3f( 1.0f,-1.0f, 1.0f);					
	glColor3f(0.0f,1.0f,0.0f);						
	glVertex3f( 1.0f,-1.0f, -1.0f);		

	// 后面
	glColor3f(1.0f,0.0f,0.0f);						
	glVertex3f( 0.0f, 1.0f, 0.0f);					
	glColor3f(0.0f,1.0f,0.0f);						
	glVertex3f( 1.0f,-1.0f, -1.0f);					
	glColor3f(0.0f,0.0f,1.0f);						
	glVertex3f(-1.0f,-1.0f, -1.0f);		

	// 左面
	glColor3f(1.0f,0.0f,0.0f);						
	glVertex3f( 0.0f, 1.0f, 0.0f);					
	glColor3f(0.0f,0.0f,1.0f);						
	glVertex3f(-1.0f,-1.0f,-1.0f);					
	glColor3f(0.0f,1.0f,0.0f);						
	glVertex3f(-1.0f,-1.0f, 1.0f);					
	glEnd();		
	glPopAttrib();							// 弹出保存的绘制属性
}

void TestGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	// This is a dummy, to avoid an endless succession of paint messages.
	// OnPaint handlers must always create a wxPaintDC.
	wxPaintDC dc(this);

	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.

	if (!m_bInitFlag)
	{
		InitGL();
		m_bInitFlag = true;
	}
	
	SetCurrent(*m_glRC);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
	glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );

	DrawTri();

	glPopMatrix();
	glFlush(); // Not really necessary: buffer swapping below implies glFlush()

	SwapBuffers();
}

void TestGLCanvas::OnSize(wxSizeEvent& event)
{
	if ( !IsShownOnScreen() )
		return;
	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.
	SetCurrent(*m_glRC);

	// It's up to the application code to update the OpenGL viewport settings.
	// This is OK here only because there is only one canvas that uses the
	// context. See the cube sample for that case that multiple canvases are
	// made current with one context.
	glViewport(0, 0, event.GetSize().x, event.GetSize().y);
}

void TestGLCanvas::OnChar(wxKeyEvent& event)
{
	switch( event.GetKeyCode() )
	{
	case WXK_ESCAPE:
		wxTheApp->ExitMainLoop();
		return;

	case WXK_LEFT:
		m_yrot -= 15.0;
		break;

	case WXK_RIGHT:
		m_yrot += 15.0;
		break;

	case WXK_UP:
		m_xrot += 15.0;
		break;

	case WXK_DOWN:
		m_xrot -= 15.0;
		break;

	default:
		event.Skip();
		return;
	}

	Refresh(false);
}

void TestGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
	static int dragging = 0;
	static float last_x, last_y;

	// Allow default processing to happen, or else the canvas cannot gain focus
	// (for key events).
	event.Skip();

	if (event.LeftIsDown())
	{
		if (!dragging)
		{
			dragging = 1;
		}
		else
		{
			m_yrot += (event.GetX() - last_x)*1.0;
			m_xrot += (event.GetY() - last_y)*1.0;
			Refresh(false);
		}
		last_x = event.GetX();
		last_y = event.GetY();
	}
	else
	{
		dragging = 0;
	}
}