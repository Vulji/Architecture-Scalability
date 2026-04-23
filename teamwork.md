Teamwork:

https://jboger.substack.com/p/the-tragedy-of-skull-and-bones

---

Dans cette post-mortem, l'auteur soulève plusieurs problèmes qui sont dus à l'organisation et au fonctionnement internes de l'équipe de développement de *Skull & Bones*. Une mauvaise gestion du scope, pas de cohérence entre les designs et les features, mauvaise communication entre les différentes équipes. On peut même extrapoler à partir de cet article une mauvaise gestion du temps de développement. Pour être plus précis, nous allons analyser les citations pour les lier directement avec les anti-patterns que l'on pourrait mettre en cause dans l'échec du jeu.

> *"Skull & Bones focuses on different areas than Black Flag, which are incredibly complex to develop. The three main culprits? Online, systemic, open-ended."*

À travers cette citation, on s'aperçoit que l'équipe de développement a over-engineered le design du jeu. Dans un besoin que l'équipe s'est créé à elle-même d'avoir quelque chose qui « surpasse Black Flag ». Parce qu'ils pensaient :

> *"Let's be honest for a minute: would people really accept a Black Flag ++ these days?"*

Alors qu'une grosse partie des joueurs eux s'attendaient justement à ça. De ce fait, l'équipe a effectué du gold plating. Une phrase de l'article qui montre que c'est le cas est la suivante :

> *"If you're wondering why AAA productions need dozens of game designers, you have the beginning of an answer: to think of all the gazillion problems that may happen and prepare monstrous plans to cover all the edge cases."*

Alors même que le système du jeu n'était pas encore stable, l'équipe effectuait du surpolissage en pensant à des scénarios imaginaires qui n'arriveraient pas, comme cette situation :

> *"What happens if your ship gets sunk while you're on a beach? Do you lose it all and respawn far away? That would be unfair… Maybe your boat is resistant when docked then, but that introduces an exploit in PVP scenarios. If we let the other sink and loot you, we could respawn you with your inventory back, so nobody gets frustrated. Oh, but we've accidentally created a duplicate glitch exploit by doing so."*

On pourrait extrapoler à partir de ça et émettre l'hypothèse que cela s'est aussi répercuté au niveau de la programmation. Les programmeurs ont sûrement over-engineered le code en pensant à ces designs pour développer des systèmes réglant tous les edge cases sans bugs, au lieu de suivre la philosophie YAGNI.

Ce problème semble être la plus grosse faiblesse du développement. Mais en découlent également d'autres problèmes : malgré les 14 années de développement, *Skull & Bones* est sorti avec de nombreux bugs. Démontrant que, malgré tous les scénarios prévus par l'équipe de développement, les features n'ont pas été implémentées de façon minutieuse. Entre ça et la complexité du système impliquée par l'auteur, il est très facile de voir que l'équipe de programmation a été traitée comme une feature factory — n'ayant pas le temps de nettoyer et maintenir leur codebase, ni d'effectuer des tests proprement.

---

De manière générale, on peut clairement voir un problème organisationnel se refléter dans cet article. Peut-être était-ce parce que l'équipe d'Ubisoft Singapour était trop peu expérimentée. Mais le jeu manquait d'une direction claire et d'un scope cohérent, faisant en sorte qu'ils soient pris dans une escalade de l'engagement. Si le jeu n'avait pas eu d'attaches légales, il aurait sûrement été annulé dans les 3 premières années.

