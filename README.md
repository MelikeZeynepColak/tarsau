Repo Link: https://github.com/MelikeZeynepColak/tarsau

# Tarsau - Simple Archive Utility

Tarsau is a lightweight archive utility developed in C for Linux/Unix systems.
It works similarly to tar, zip, or rar, but without compression.

The program combines multiple ASCII text files into a single `.sau` archive file and can later extract them while preserving original file permissions.

This project was developed as a Systems Programming course assignment using low-level file operations and system calls in C.

---

# Features

- Archive multiple ASCII text files into a single `.sau` file
- Extract archived files back to the filesystem
- Preserve original file permissions (read/write/execute)
- Support up to 32 input files
- Maximum archive size: 200 MB
- ASCII text file validation
- Relative or absolute extraction directory support
- Graceful error handling without unexpected crashes

---

# Project Structure

.
├── src/            # Source files
├── include/        # Header files
├── lib/            # Additional libraries (if any)
├── Makefile
└── README.md

---

# Build Instructions

Compile the project using:

make

After successful compilation, the executable file will be created:

./tarsau

---

# Usage

## 1. Create Archive (`-b`)

Basic syntax:

./tarsau -b [input_files] -o [archive_name]

If `-o` is not specified, the default archive name is:

a.sau

### Example

./tarsau -b file1.txt file2.txt notes.dat -o backup.sau

This command creates:

backup.sau

containing all given text files.

### Default Output Example

./tarsau -b file1.txt file2.txt

Output archive:

a.sau

---

## 2. Extract Archive (`-a`)

Basic syntax:

./tarsau -a archive.sau [directory]

### Examples

Extract into current directory:

./tarsau -a backup.sau

Extract into a new directory:

./tarsau -a backup.sau extracted_files

If the target directory does not exist, it is automatically created.

---

# Archive File Format

The `.sau` archive consists of two sections:

## 1. Metadata Section

- First 10 bytes store the size of the metadata section in ASCII format.
- File records are separated using `|`.
- Each record format:

|filename,permissions,size|

Example:

|notes.txt,644,120|

---

## 2. File Data Section

After metadata ends:

- File contents are stored sequentially
- No separators are used between file contents
- All contents are stored as ASCII text

---

# Validation Rules

The program validates:

- Input files must be ASCII text files
- Total input size must not exceed 200 MB
- Maximum number of files is 32
- Archive extension must be `.sau`
- Corrupted or invalid archives are detected

---

# Error Handling

Examples of handled errors:

## Invalid Input File

t7 input file format is incompatible!

## Corrupted Archive

Archive file is invalid or corrupted!

The program exits safely without crashing.

---

# Permission Preservation

During extraction, original file permissions are restored using Linux file permission mechanisms.

Example:

-rwxr-xr--

Permissions before archiving and after extraction remain identical.

---

# Technologies Used

- C Programming Language
- Linux System Calls
- File I/O Operations
- POSIX File Permission APIs
- Makefile Build System

---

# Example Workflow

## Create Archive

./tarsau -b t1.txt t2.txt t3.txt -o myarchive.sau

## Extract Archive

./tarsau -a myarchive.sau output_folder

---

# Limitations

- Only ASCII text files are supported
- No compression is applied
- Binary files are rejected
- Maximum archive size is 200 MB

---

# Authors

- Melike Zeynep Çolak

---

