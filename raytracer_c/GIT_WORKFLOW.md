# GIT_WORKFLOW.md - Git Strategy for Raytracer

## 🎯 Philosophy: 1 Feature = 1 Clean Commit

Chaque commit représente **une feature complète et testable**, pas juste "quelques lignes de code".

---

## 📋 Setup Initial

```bash
# Initialiser repo
cd raytracer_c
git init
git config user.name "Your Name"
git config user.email "your@email.com"

# Ajouter tous les fichiers de setup
git add README.md SPECIFICATION.md COPILOT.md PROJECT_STRUCTURE.md Makefile .gitignore
git commit -m "init: project setup with docs and build system"

# Vérifier
git log --oneline
```

---

## 🔄 Workflow par Commit

### Option A : Feature Branch (Recommended)

**Avantage** : Travail isolé, facile à revert, review avant merge.

```bash
# 1. Créer branche pour feature
git checkout -b feat/vector-math

# 2. Développer avec l'agent IA
# ... codegen ...
mkdir -p src/math
# ... fichiers créés ...

# 3. Vérifier compilation
make clean && make
./build/bin/raytracer output.ppm
# Vérifier output.ppm

# 4. Review le code
git status        # Vérifier fichiers
git diff          # Vérifier contenu (si déjà staged)

# 5. Stage et commit
git add src/math/
git commit -m "feat: add vector math module"

# Message: "feat: ..." (voir conventions ci-dessous)

# 6. Merger dans main
git checkout main
git merge feat/vector-math

# 7. Nettoyer branche locale
git branch -d feat/vector-math
```

### Option B : Direct Commit (Faster)

**Avantage** : Plus rapide, moins de branching.

```bash
# 1. Développer (sur main)
# ... codegen ...

# 2. Vérifier + tester
make clean && make
./build/bin/raytracer output.ppm

# 3. Stage et commit
git add src/math/
git commit -m "feat: add vector math module"
```

---

## 📝 Commit Message Convention

**Format** : `<type>: <description>`

### Types

| Type | Usage | Phase |
|------|-------|-------|
| `feat:` | Nouvelle feature | Toutes |
| `perf:` | Optimisations | Phase 5+ |
| `refactor:` | Restructuration | Après stabilité |
| `fix:` | Bug fixes | Au besoin |
| `docs:` | Documentation | Toutes |
| `init:` | Setup initial | Début |

### Exemples corrects

```
feat: add vector math module
feat: implement sphere intersection
feat: add point light support
feat: implement BVH acceleration structure
perf: add SIMD optimizations for vec3
perf: implement memory pooling allocator
fix: correct normal calculation in sphere intersection
docs: update Phase 1 quickstart guide
```

### Exemples incorrects ❌

```
WIP: coding
update
fix stuff
added things
v2
```

---

## 🔍 Code Review avant Commit

Avant `git commit`, vérifier :

```bash
# 1. Compilation
make clean && make
# ✓ Zéro warnings/errors

# 2. Runtime
./build/bin/raytracer output.ppm
# ✓ Pas de crash/segfault

# 3. Vérifier diff
git diff HEAD

# Checklist :
# [ ] Code compilable
# [ ] Tests fonctionnels
# [ ] Pas de malloc/free non-géré (sauf si voulu)
# [ ] Pas de TODOs/XXX
# [ ] Pas de code commenté inutile
# [ ] Headers clairs
# [ ] Nommage cohérent

# 4. Staging sélectif (si plusieurs fichiers)
git add src/math/vec3.h
git add src/math/vec3.c
git status  # Vérifier que c'est bon

# 5. Commit
git commit -m "feat: add vector math module"
```

---

## 📊 Viewing History

### Log lisible

```bash
# Simple et clair
git log --oneline

# Avec graphe (si branches)
git log --oneline --graph --all

# Détaillé
git log -p  # Montre le diff de chaque commit
git log --stat  # Montre fichiers modifiés

# Example output:
# 5f3a2c1 feat: add vector math module
# 1e2b8c9 init: project setup with docs and build system

```

### Voir les changements

```bash
# Entre deux commits
git diff <commit1> <commit2>

# Entre main et branche
git diff main feat/vector-math

# Voir un commit spécifique
git show 5f3a2c1
```

---

## 🔄 Workflow Phase par Phase

### Phase 1: 6 commits

```
init: project setup
feat: add vector math module
feat: add ray structure
feat: implement basic camera
feat: add PPM image output
feat: implement sphere intersection
feat: add flat shading and main loop
```

### Phase 2: 4 commits

```
feat: add point light support
feat: implement lambert diffuse shading
feat: add phong specular highlights
feat: implement shadow rays
```

### Phase 3: 3 commits

```
feat: add recursive reflections
feat: implement refraction with snell's law
feat: add fresnel effect
```

**Total attendu** : ~30-35 commits clairs et logiques.

---

## ⚙️ Advanced Git Techniques

### Amender un commit

Si vous oubliez un fichier ou une modification :

```bash
# Faire la modification
git add fichier_oublie.c

# Amender le commit précédent
git commit --amend

# ⚠️ ATTENTION : si déjà pushé, éviter --force sur main
git push origin HEAD --force-with-lease
```

### Stash temporaire

Si vous devez switcher de branche sans committer :

```bash
git stash  # Sauvegarder WIP

# ... faire autre chose ...

git stash pop  # Récupérer le stash
```

### Revert un commit

Si un commit cause des problèmes :

```bash
# Revenir EN ARRIERE (crée nouveau commit)
git revert <commit-id>

# Ou annuler le commit (reset, perte de code!)
git reset --hard <commit-id>  # ⚠️ Dangereux !
```

### Cherry-pick

Prendre un commit d'une branche et l'appliquer à une autre :

```bash
git checkout main
git cherry-pick feat/some-feature
```

---

## 🚀 Working with Multiple Agents

Si vous utilisez plusieurs agents IA en parallèle (bonus) avec Git worktrees :

```bash
# Créer worktree pour branche parallèle
git worktree add ../raytracer-phase2 -b phase2-lighting

# Maintenant deux répertoires :
# raytracer_c/          (main/phase1)
# raytracer-phase2/     (phase2-lighting)

# Développer indépendamment dans chaque, puis merger :
cd ../raytracer_c
git merge ../raytracer-phase2

# Nettoyer
git worktree remove ../raytracer-phase2
```

---

## 📁 .gitignore Respect

Les fichiers suivants ne doivent PAS être committés :

```
build/               # Artefacts compilés
*.o, *.a, *.so      # Object files
*.ppm, *.png         # Images générées
gmon.out             # Profiling
*.swp, *~            # Éditeur temporaires
.vscode/             # IDE spécific
```

Vérifier :
```bash
git status

# Ne doit montrer QUE les fichiers .c/.h/.md/Makefile
# Si des build/ ou *.ppm apparaissent, vérifier .gitignore
```

---

## 🔗 Branches Convention

| Branch | Purpose |
|--------|---------|
| `main` | Release-ready, toujours compilable |
| `feat/*` | Feature branches (court-lived) |
| `dev` | (Optional) Integration branch |

```bash
git branch          # Lister branches locales
git branch -r       # Lister branches remote
git branch -a       # Toutes les branches
```

---

## 📦 Pre-Push Checklist

Avant un `git push` (si vous poussez vers GitHub) :

```bash
# 1. Dernier commit est bon
git log --oneline -5

# 2. Compilation et test OK
make clean && make
./build/bin/raytracer output.ppm

# 3. Pas de fichiers non-tracked problématiques
git status

# 4. Push
git push origin main
# ou
git push origin feat/feature-name
```

---

## 🎓 Git Learning Resources

- **Interactive** : https://learngitbranching.js.org/
- **Docs** : https://git-scm.com/doc
- **Cheatsheet** : https://github.github.com/training-kit/

---

## 💾 Backup & Recovery

### Sauvegarder localement

```bash
# Avant de faire du git reset dangereux :
git branch backup/current-state
```

### Récupérer un commit perdu

```bash
git reflog  # Voir tous les commits (même supprimés)
git checkout <perdu-commit-id>
```

---

## 🎬 Final Session Summary

À la fin (après Phase 7) :

```bash
# Voir tous les commits
git log --oneline

# Générer un summary
git log --oneline > GIT_HISTORY.txt

# Créer SESSION.md avec :
# - Git log
# - Nombre de commits par phase
# - Temps total de dev
# - Performance improvement mesurée
```

---

## ✅ Checklist Git Final

- [ ] Tous les commits ont des messages clairs
- [ ] Pas de "WIP" ou "fix" sans contexte
- [ ] 30+ commits logiques et testables
- [ ] Pas de fichiers build/ ou .ppm committés
- [ ] Main branche toujours compilable
- [ ] README.md à jour avec final stats
- [ ] GIT_HISTORY.txt généré

---

Happy committing! 🚀
