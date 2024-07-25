#!/usr/bin/env python3

import subprocess
import argparse
import sys
import os


types: list[str] = [
	f'dev_server',
	f'rel_server',
	f'dev_client',
	f'rel_client',
	f'dev_all',
	f'rel_all',
	f'all',
]

build_c_name: str = f'build.c'
build_bin_name: str = f'build.bin'
build_bin_old_name: str = f'build.bin.old'


def bootstrap_build_system(project_dir: str) -> bool:
	build_bin_path: str = os.path.join(project_dir, build_bin_name)

	if not os.path.isfile(build_bin_path):
		print(f'info : removing build system.')
		old_build_bin_path: str = os.path.join(project_dir, build_bin_old_name)
		if os.path.exists(old_build_bin_path):
			os.remove(old_build_bin_path)
		
		print(f'info : bootstrapping the build system.')
		result: subprocess.CompletedProcess[bytes] = subprocess.run(
			[f'gcc', f'-std=gnu11', f'-Wall', f'-Wextra', f'-Werror', f'-o', build_bin_path, os.path.join(project_dir, build_c_name)]
		)
		if result.returncode != 0:
			print(f'error: failed to bootstrap the build system.')
			return False

		os.chmod(build_bin_path, 0o755)
	return True


def clean(project_dir: str) -> bool:
	if not bootstrap_build_system(project_dir):
		return False

	print(f'info : cleaning the project.')
	result: subprocess.CompletedProcess[bytes] = subprocess.run(
		[os.path.join(project_dir, build_bin_name), f'clean'], cwd=project_dir
	)
	if result.returncode != 0:
		return False

	return True


def build(build_type: str, project_dir: str) -> bool:
	global types
	if build_type not in types:
		print(f'error: build configuration must be one of the {types}')
		return False

	if not bootstrap_build_system(project_dir):
		return False

	print(f'info : building the project.')
	result: subprocess.CompletedProcess[bytes] = subprocess.run(
		[os.path.join(project_dir, build_bin_name), f'build_{build_type}'], cwd=project_dir
	)
	if result.returncode != 0:
		return False

	return True


def lint(lint_type: str, project_dir: str) -> bool:
	global types
	if lint_type not in types:
		print(f'error: lint configuration must be one of the {types}')
		return False

	if not bootstrap_build_system(project_dir):
		return False

	print(f'info : linting the project for {lint_type}.')
	result: subprocess.CompletedProcess[bytes] = subprocess.run(
		[os.path.join(project_dir, build_bin_name), f'lint_{lint_type}'], cwd=project_dir
	)
	if result.returncode != 0:
		return False

	return True


def docs(project_dir: str) -> bool:
	if not bootstrap_build_system(project_dir):
		return False

	print(f'info : generating documentation.')
	result: subprocess.CompletedProcess[bytes] = subprocess.run(
		[os.path.join(project_dir, build_bin_name), f'docs'], cwd=project_dir
	)
	if result.returncode != 0:
		return False

	return True


def run(run_type: str, project_dir: str) -> bool:
	global types
	if run_type not in types:
		print(f'error: run configuration must be one of the {types}')
		return False

	if not bootstrap_build_system(project_dir):
		return False

	print(f'info : running the project for {run_type}.')
	result: subprocess.CompletedProcess[bytes] = subprocess.run(
		[os.path.join(project_dir, build_bin_name), f'run_{run_type}'], cwd=project_dir
	)
	if result.returncode != 0:
		return False

	return True


def main() -> None:
	global types
	global types

	parser: argparse.ArgumentParser = argparse.ArgumentParser()
	subparsers: argparse._SubParsersAction[argparse.ArgumentParser] = parser.add_subparsers(dest=f'command', required=True, help=f'Command to execute')
	clean_parser: argparse.ArgumentParser = subparsers.add_parser(f'clean', help=f'The clean command')
	build_parser: argparse.ArgumentParser = subparsers.add_parser(f'build', help=f'The build command')
	build_parser.add_argument(f'--type', choices=types, type=str, default='all', help=f'Build type')
	lint_parser:  argparse.ArgumentParser = subparsers.add_parser(f'lint', help=f'The lint command')
	lint_parser.add_argument(f'--type', choices=types, type=str, default='all', help=f'Lint type')
	docs_parser:  argparse.ArgumentParser = subparsers.add_parser(f'docs', help=f'The docs command')
	run_parser:  argparse.ArgumentParser = subparsers.add_parser(f'run', help=f'The run command')
	run_parser.add_argument(f'--type', choices=types, type=str, default='all', help=f'Run type')
	args: argparse.Namespace = parser.parse_args()

	script_path: str = os.getcwd()
	project_dir: str = os.path.dirname(script_path)
	os.chdir(project_dir)

	if args.command == f'clean':
		if not clean(project_dir):
			sys.exit(1)
	elif args.command == f'build':
		if not build(args.type, project_dir):
			sys.exit(1)
	elif args.command == f'lint':
		if not lint(args.type, project_dir):
			sys.exit(1)
	elif args.command == f'docs':
		if not docs(project_dir):
			sys.exit(1)
	elif args.command == f'run':
		if not run(args.type, project_dir):
			sys.exit(1)
	else:
		print(f'error: invalid command \'{args.command}\'')
		parser.print_help()
		sys.exit(1)


if __name__ == f'__main__':
	main()
