import * as tsc from 'typescript';
import { promises as fs } from 'fs';
import path from 'path';
import chokidar from 'chokidar';

// Function to transpile a TypeScript file to ES5
async function transpileFile(filePath) {
  const tsConfig = {
    compilerOptions: {
      target: tsc.ScriptTarget.ES2024,
      outDir: path.dirname(filePath),
    },
  };

  const source = await fs.readFile(filePath, 'utf8');
  const result = tsc.transpileModule(source, tsConfig);

  const outputFilePath = filePath.replace(/\.ts$/, '.js').replace(/ts[\/|\\]/, 'js/');
  await fs.mkdir(path.dirname(outputFilePath), { recursive: true });
  await fs.writeFile(outputFilePath, result.outputText, 'utf8');

  console.log(`Transpiled ${filePath} to ${outputFilePath}`);
}

const directoryPath = 'ts';

chokidar.watch(directoryPath, {
  persistent: true,
  ignored: (file, stats) =>
    stats?.isFile() && file.endsWith('.d.ts') ||
    stats?.isFile() && !file.endsWith('.ts'),
})
  .on('add', transpileFile)
  .on('change', transpileFile)
  .on('unlink', (filePath) => {
    const outputFilePath = filePath.replace(/\.ts$/, '.js').replace(/game[\/|\\]ts/, 'game/js');
    fs.unlink(outputFilePath).then(() => {
      console.log(`Deleted ${outputFilePath}`);
    }).catch((err) => {
      console.log(`Error deleting ${outputFilePath}: ${err}`);
    });
  });

console.log(`Watching for file changes in ${directoryPath}`);
