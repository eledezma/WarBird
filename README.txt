Comp465: Project 1 README

Project Member Names: Ernie Ledezma, Stanley Kirdey

Instructions:  Open up the .sln project and execute the .cpp file!

Summary:  Overall everything for Phase 1 was accomplished.  First and foremost we used layed out our code based on the ManyModelsStatic and ManyCubes projects.  We decide to used Shape3D.hpp class from ManyCubes static since we found that loading models though it was much more efficient.  It would also make our project more modular and therefore easier to edit for future phases.  Lastly we also saw there was some rotation logic in Shape3D that could help us with our orbital rotations.
	As our project Stands we have loaded 7 models, 5 of which are all spheres and the other is the custom model “Warbird”.  The moons and planets seem to be orbiting correctly and are at the right spot.  For our Missile model we used a the “battleship” model since it looked more like missile.
	Each View is controlled by pressing “v”.  At first the project is loaded in an initial view.  When the user hits v for the first time he enter the “toggle loop” which starts out with “Front View”.  So it might seem like hitting “v” did nothing the first time, but it did.  t simply switched from default view, to the first view on the toggle loop, which is “Front View”
