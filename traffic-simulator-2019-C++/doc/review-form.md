# Review form for project XXX

**Name of project to be reviewed:**  traffic-simulator-2019-1

**Names of reviewers:** Akseli Oinaanoja, Mikael Tatun, Kaisa Kärkkäinen, Tommi Gröhn

Provide short comments (2-4 sentences) for each item below.

## 1. Overall design and functionality (0-6p)

  * 1.1: The implementation corresponds to the selected topic and
scope. The extent of project is large enough to accommodate work for
everyone (2p)

The interactive graphical interface is really neat! Traffic analysis seems to be a WIP at the moment. A ton of extra features planned, so finding work for everyone probably won't be much of a problem.

  * 1.2: The software structure is appropriate, clear and well
documented. e.g. class structure is justified, inheritance used where
appropriate, information hiding is implemented as appropriate. (2p)

Looking at the plan, the project seems to be very well thought-out. Class structure is clear, and inheritance is used where appropriate.

  * 1.3: Use of external libraries is justified and well documented. (2p)

Yes.

## 2. Working practices (0-6p)

  * 2.1: Git is used appropriately (e.g., commits are logical and
frequent enough, commit logs are descriptive). (2 p)

There are already more than 100 commits, most if not all well documented and descriptive ones. Commits are definitely frequent enough. Git page looks understandable and navigatable even for an outsider.


  * 2.2: Work is distributed and organised well. Everyone contributes to
the project and has a relevant role that matches his/her skills. The
distribution of roles is described well enough. (2p)

Loads of documentation and scheduling is obviously involved and the project managing leaves a professional impression. There are tables describing different tasks with each member more or less appropriately assigned for them. Role management seems justified enough.


  * 2.3: Quality assurance is appropriate. Implementation is tested
comprehensively and those testing principles are well documented. (2p)


Testing seems to be a crucial part of the implementation in this project. Testing probably could be mentioned more extensively in the documentation but then again the simulation demo worked. Comments are well used in the code.


## 3. Implementation aspects (0-8p)

  * 3.1: Building the software is easy and well documented. CMake or
such tool is highly recommended. (2p)

CMake is used for building and apparently works.

  * 3.2: Memory management is robust, well-organised and
coherent. E.g., smart pointers are used where appropriate or RO3/5 is
followed. The memory management practices should be documented. (2p)

Smart pointers are extensively utilized, though there didn't seem to be any memory management at runtime (where them destructors? or am i just blind?)

  * 3.3: C++ standard library is used where appropriate. For example,
containers are used instead of own solutions where it makes sense. (2
p)

Yes.

  * 3.4: Implementation works robustly also in exceptional
situations. E.g., functions can survive invalid inputs and exception
handling is used where appropriate. (2p)

Implementation works robustly in the sense that it doesn't seem to really allow invalid user inputs. So not much need for exception handling.

## 4. Project extensiveness (0-10p)

  * Project contains features beyond the minimal requirements: Most of
the projects list additional features which can be implemented for
more points. Teams can also suggest their own custom features, though
they have to be in the scope of the project and approved by the course
assistant who is overseeing the project. (0-10p)

As for the additional features, the project does have e.g. traffic lights that work already. It also has the ability to create an arbitrary simulation (map editor) and manages to avoid possible bugs when doing so. The option menu is comprehensive and understandable for the user.
A lot of additional features are mentioned in the adfeatures.png file in Git but of course not everything is implemented yet. Obviously this means that the grading between 0-10p can't really be done here.
