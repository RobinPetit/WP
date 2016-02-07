# INFOF-209
Wizard Poker

## Compilation

It is better to do an out-of-source build:

    mkdir build && cd build
	cmake ..
	make


## Usual workflow

When you want to contribute, here is an idea of the usual workflow:

    git pull
	git checkout -b nameOfYourModifications
	# Do things in you local branch
	git add *filesThatYouJustModified*
	git commit
	# Do more commits...
	git push

Then open a pull request on GitHub for the branch nameOfYourModifications,
and if you coworker(s) agree(s), merge it (with the GitHub interface, for example).


## Remarks on Debriefing 1

Here is the remarks I noted while the debriefing on Tuesday 02/02/2016 :

+ the SRD is not complete and too short ;
+ the formatting have to be improved (rewrite the flyleaf) ;
+ in the deck management diagram, explain the notion of default deck (which is not empty);
+ write preconditions and postconditions for **every** use cases diagrams ;
+ in the gameplay diagram, the case when the monster does not die is missing;
+ ambiguity of the class *Compte* ;

