
# Git GUI Application

This is a Qt-based GUI application for managing Git repositories. The application allows users to perform common Git operations such as creating repositories, cloning repositories, adding and committing changes, pushing and pulling from remotes, and more, all from a user-friendly graphical interface.

![image](https://github.com/NadaMohamedMoharram/GIT-GUI-using-Qt/assets/120272090/d82422c4-0def-4f16-83af-2d3dd21a0f70)

## Features

- **Create Repository**: Initialize a new Git repository.
- **Clone Repository**: Clone an existing Git repository from a remote URL.
- **Add Changes**: Stage all changes in the repository.
- **Commit Changes**: Commit staged changes with a user-provided commit message.
- **Push Changes**: Push commits to a remote repository.
- **Pull Changes**: Pull changes from a remote repository.
- **Branch Management**: Create, switch, and list branches.
- **Merge Branches**: Merge a specified branch into the current branch.
- **View Logs**: View the commit logs of the repository.
- **Browse for Directory**: Select a directory for Git operations using a file dialog.

## Requirements

- Qt 5.12 or later
- C++17 or later
- Git

## Installation

1. **Clone the Repository**:
   ```bash
   git clone <repository-url>
   ```

2. **Open the Project**:
   - Open the project in Qt Creator.

3. **Build the Project**:
   - Configure the project using the appropriate kit.
   - Build the project.

4. **Run the Application**:
   - Run the built application.

## Usage

1. **Initialize a Repository**:
   - Enter the path where you want to create the repository.
   - Click the "Create Repo" button.

2. **Clone a Repository**:
   - Enter the local path and the remote repository URL.
   - Click the "Clone" button.

3. **Add Changes**:
   - Click the "Add All" button to stage all changes in the repository.

4. **Commit Changes**:
   - Click the "Commit" button.
   - Enter a commit message in the prompt that appears.

5. **Push Changes**:
   - Enter the remote name and branch in the respective fields.
   - Click the "Push" button.

6. **Pull Changes**:
   - Enter the remote name and branch in the respective fields.
   - Click the "Pull" button.

7. **Create a Branch**:
   - Click the "Create Branch" button.
   - Enter the branch name in the prompt that appears.

8. **Switch Branch**:
   - Click the "Switch Branch" button.
   - Enter the branch name to switch to in the prompt that appears.

9. **Merge Branch**:
   - Click the "Merge Branch" button.
   - Enter the branch name to merge in the prompt that appears.

10. **View Logs**:
    - Click the "Log" button to view the commit logs.

## Screenshots

![image](https://github.com/NadaMohamedMoharram/GIT-GUI-using-Qt/assets/120272090/64161eb0-5a74-4d4b-9dc1-b194aa81e40a)


![image](https://github.com/NadaMohamedMoharram/GIT-GUI-using-Qt/assets/120272090/759fd2ee-8edf-471d-a7c5-fe23ffd0bc90)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [Qt](https://www.qt.io/) - Cross-platform software development framework.
- [Git](https://git-scm.com/) - Version control system.
```

Make sure to replace `<repository-url>` with your actual repository URL and update the paths for screenshots if you have any. Additionally, you can add more detailed instructions or sections as needed.
