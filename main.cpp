#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLogger>
#include <Wt/WOverlayLoadingIndicator>
#include "ui/project1.h"
using namespace Wt;

class Comphoto2011Application : public WApplication {
public:
	Comphoto2011Application(const WEnvironment &env) : WApplication(env) {
		setTitle("Computational Photography, Fall 2011");
		setCssTheme("polished");
		setLoadingIndicator(new WOverlayLoadingIndicator());
		messageResourceBundle().use(appRoot() + "static/project1");
		root()->setPadding(10);
		root()->resize(WLength::Auto, WLength::Auto);

		new Project1(root());
		useStyleSheet(appRoot() + "static/project1.css");
	}
};


WApplication *createApplication(const WEnvironment &env)
{
	return new Comphoto2011Application(env);
}


int main(int argc, char **argv)
{
	return WRun(argc, argv, &createApplication);
}
