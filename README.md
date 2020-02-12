*************************** NOTES ***************************

**Master <- Stage <- Dev <- Feature**

**Pour developper une nouvelle fonctionnalite:**

-> git checkout dev (se rendre dans la brache dev)

-> git branch <my_feature> (cree une branch depuis dev)


**Lorsque ma fonctionnalite est operationelle:**

-> git checkout dev (se rendre sur la branche dev)

-> git merge <my_feature> (fusionner ma branche sur dev)


**Lorsqu'on a termine une etape du projet:**

-> git checkout stage (se rendre sur la branche stage)

-> git merge dev (fusionner ma branche sur stage depuis dev)


**Lorsque tout est a la norme et que les test sont valides:**

-> git checkout master (se rendre sur la branche master)

-> git merge stage (fusionner ma branche sur master depuis stage)


*************************** WARNING ***************************

```
	!!! Pour merge toujours se rendre d'abord !!!
!!! sur la branche qui va recevoir les modifications !!!

```
*************************** WARNING ***************************

